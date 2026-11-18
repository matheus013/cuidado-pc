#include "backend.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStorageInfo>
#include <QSysInfo>
#include <QtConcurrentRun>

Backend::Backend(QObject *parent) : QObject(parent), m_summary(QStringLiteral("Inicie uma análise para revisar os arquivos temporários.")), m_taskTitle(QStringLiteral("Verificações")), m_taskOutput(QStringLiteral("Escolha uma verificação. Nenhum reparo ou atualização será instalado automaticamente.")) {
    m_process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &Backend::appendOutput);
    connect(&m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError e) {
        if (e == QProcess::FailedToStart) { m_taskOutput += QStringLiteral("\nNão foi possível iniciar: ") + m_process.errorString(); emit changed(); }
    });
    connect(&m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](int code, QProcess::ExitStatus status) {
        appendOutput(); m_taskOutput += status == QProcess::NormalExit ? QStringLiteral("\n\nProcesso encerrado (código %1). Leia a saída acima; código 0 não garante que tudo está atualizado ou íntegro.").arg(code) : QStringLiteral("\n\nProcesso interrompido."); emit changed();
    });
    connect(&m_watcher, &QFutureWatcher<ScanResult>::finished, this, [this] {
        auto result = m_watcher.result(); m_scanning = false; m_files = result.files; m_selected.clear();
        qint64 total = 0; for (const auto &f : m_files) total += f.size;
        m_summary = result.error.isEmpty() ? QStringLiteral("%1 arquivos encontrados · %2%3. Acesso negado a pastas pode omitir arquivos.").arg(m_files.size()).arg(humanSize(total), result.truncated ? QStringLiteral(" · limite de 10.000 atingido") : QString()) : result.error;
        emit changed();
    });
    refresh();
}
QVariantList Backend::files() const { QVariantList r; for (int i = 0; i < m_files.size(); ++i) { const auto &f = m_files[i]; r.append(QVariantMap{{"path", f.path}, {"size", humanSize(f.size)}, {"date", f.modified.toLocalTime().toString("dd/MM/yyyy")}, {"selected", m_selected.contains(i)}, {"index", i}}); } return r; }
QString Backend::selectedSize() const { qint64 n = 0; for (int i : m_selected) if (i >= 0 && i < m_files.size()) n += m_files[i].size; return humanSize(n); }
void Backend::refresh() {
    QStorageInfo disk(QDir::homePath());
    m_diagnostic = QStringLiteral("Sistema: %1\nComputador: %2\nDisco total: %3  •  Livre: %4\nPasta temporária: %5").arg(QSysInfo::prettyProductName(), QSysInfo::machineHostName(), disk.isValid() && disk.isReady() ? humanSize(disk.bytesTotal()) : QStringLiteral("indisponível"), disk.isValid() && disk.isReady() ? humanSize(disk.bytesAvailable()) : QStringLiteral("indisponível"), QDir::tempPath()); emit changed();
}
void Backend::scan() {
    if (m_scanning) return;
    m_scanning = true; m_files.clear(); m_selected.clear(); m_summary = QStringLiteral("Analisando arquivos temporários…"); emit changed();
    const auto root = QDir::tempPath(); m_watcher.setFuture(QtConcurrent::run([root] { return scanRecursive(root); }));
}
void Backend::setSelected(int index, bool selected) { if (index < 0 || index >= m_files.size()) return; if (selected) m_selected.insert(index); else m_selected.remove(index); emit changed(); }
void Backend::selectAll(bool selected) { m_selected.clear(); if (selected) for (int i = 0; i < m_files.size(); ++i) m_selected.insert(i); emit changed(); }
void Backend::clean() {
    if (m_scanning || m_selected.isEmpty()) return;
    QList<FileEntry> chosen; for (int i : m_selected) if (i >= 0 && i < m_files.size()) chosen.append(m_files[i]);
    const auto result = cleanFiles(QDir::tempPath(), chosen); m_files.clear(); m_selected.clear();
    m_summary = QStringLiteral("Limpeza: %1 removidos; %2 ignorados ou inacessíveis. Faça uma nova análise.").arg(result.removed).arg(result.skipped); refresh();
}
void Backend::appendOutput() { m_taskOutput += QString::fromLocal8Bit(m_process.readAllStandardOutput()); if (m_taskOutput.size() > 200000) m_taskOutput = QStringLiteral("[Saída anterior truncada]\n") + m_taskOutput.right(190000); emit changed(); }
void Backend::runTask(const QString &title, const QString &program, const QStringList &args) {
    if (running()) return;
    m_taskTitle = title; m_taskOutput = QStringLiteral("Executando %1 %2\n\n").arg(program, args.join(' '));
    m_process.start(program, args); emit changed();
}
void Backend::verifySystem() {
#ifdef Q_OS_WIN
    const auto exe = QDir(qEnvironmentVariable("SystemRoot", "C:/Windows")).filePath("System32/sfc.exe");
    runTask(QStringLiteral("Arquivos protegidos do Windows (inclui DLLs do sistema)"), exe, {QStringLiteral("/verifyonly")});
#elif defined(Q_OS_MACOS)
    runTask(QStringLiteral("Verificação do volume do macOS"), QStringLiteral("/usr/sbin/diskutil"), {QStringLiteral("verifyVolume"), QStringLiteral("/")});
#else
    m_taskTitle = QStringLiteral("Verificação não disponível"); m_taskOutput = QStringLiteral("Esta verificação está disponível apenas no Windows ou macOS."); emit changed();
#endif
}
void Backend::checkUpdates() {
#ifdef Q_OS_WIN
    const auto path = QStandardPaths::findExecutable(QStringLiteral("winget"));
    if (path.isEmpty()) { m_taskTitle = QStringLiteral("Atualizações"); m_taskOutput = QStringLiteral("winget não encontrado. Consulte o Windows Update e as lojas dos aplicativos."); emit changed(); return; }
    runTask(QStringLiteral("Atualizações de aplicativos via winget"), path, {QStringLiteral("upgrade"), QStringLiteral("--disable-interactivity")});
#elif defined(Q_OS_MACOS)
    runTask(QStringLiteral("Atualizações do macOS"), QStringLiteral("/usr/sbin/softwareupdate"), {QStringLiteral("--list")});
#else
    m_taskTitle = QStringLiteral("Atualizações não disponíveis"); m_taskOutput = QStringLiteral("Disponível apenas no Windows ou macOS."); emit changed();
#endif
}
void Backend::cancelTask() { if (running()) { m_process.kill(); m_taskOutput += QStringLiteral("\nCancelamento solicitado."); emit changed(); } }
