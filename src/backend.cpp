#include "backend.h"
#include <QDir>
#include <QFileInfo>
#include <QStorageInfo>
#include <QSysInfo>

Backend::Backend(QObject *parent) : QObject(parent), m_summary(QStringLiteral("Clique em “Analisar temporários” para começar.")) { refresh(); }
QVariantList Backend::files() const {
    QVariantList result;
    for (const auto &f : m_files) result.append(QVariantMap{{"path", f.path}, {"size", humanSize(f.size)}});
    return result;
}
void Backend::refresh() {
    QStorageInfo disk(QDir::homePath());
    m_diagnostic = QStringLiteral("Sistema: %1\nComputador: %2\nDisco total: %3  •  Livre: %4\nPasta temporária: %5")
        .arg(QSysInfo::prettyProductName(), QSysInfo::machineHostName(), disk.isValid() && disk.isReady() ? humanSize(disk.bytesTotal()) : QStringLiteral("indisponível"), disk.isValid() && disk.isReady() ? humanSize(disk.bytesAvailable()) : QStringLiteral("indisponível"), QDir::tempPath());
    emit changed();
}
void Backend::scan() {
    const QDir dir(QDir::tempPath());
    m_files.clear();
    if (!dir.exists() || !QFileInfo(dir.path()).isReadable()) {
        m_summary = QStringLiteral("Não foi possível acessar a pasta temporária.");
    } else {
        m_files = scanFiles(dir.path());
        qint64 total = 0; for (const auto &f : m_files) total += f.size;
        m_summary = QStringLiteral("%1 arquivos · %2").arg(m_files.size()).arg(humanSize(total));
    }
    emit changed();
}
void Backend::clean() {
    const auto result = cleanFiles(QDir::tempPath(), m_files);
    m_files.clear();
    m_summary = QStringLiteral("Limpeza concluída: %1 removidos; %2 ignorados ou inacessíveis.").arg(result.removed).arg(result.skipped);
    refresh();
}
