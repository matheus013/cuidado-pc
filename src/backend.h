#pragma once
#include "cleanup.h"
#include <QObject>
#include <QVariantList>
#include <QSet>
#include <QProcess>
#include <QFutureWatcher>

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString diagnostic READ diagnostic NOTIFY changed)
    Q_PROPERTY(QString summary READ summary NOTIFY changed)
    Q_PROPERTY(QVariantList files READ files NOTIFY changed)
    Q_PROPERTY(int count READ count NOTIFY changed)
    Q_PROPERTY(int selectedCount READ selectedCount NOTIFY changed)
    Q_PROPERTY(QString selectedSize READ selectedSize NOTIFY changed)
    Q_PROPERTY(bool scanning READ scanning NOTIFY changed)
    Q_PROPERTY(bool running READ running NOTIFY changed)
    Q_PROPERTY(QString taskTitle READ taskTitle NOTIFY changed)
    Q_PROPERTY(QString taskOutput READ taskOutput NOTIFY changed)
public:
    explicit Backend(QObject *parent = nullptr);
    QString diagnostic() const { return m_diagnostic; }
    QString summary() const { return m_summary; }
    QVariantList files() const;
    int count() const { return int(m_files.size()); }
    int selectedCount() const { return int(m_selected.size()); }
    QString selectedSize() const;
    bool scanning() const { return m_scanning; }
    bool running() const { return m_process.state() != QProcess::NotRunning; }
    QString taskTitle() const { return m_taskTitle; }
    QString taskOutput() const { return m_taskOutput; }
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void scan();
    Q_INVOKABLE void setSelected(int index, bool selected);
    Q_INVOKABLE void selectAll(bool selected);
    Q_INVOKABLE void clean();
    Q_INVOKABLE void verifySystem();
    Q_INVOKABLE void checkUpdates();
    Q_INVOKABLE void cancelTask();
signals:
    void changed();
private:
    void runTask(const QString &title, const QString &program, const QStringList &args);
    void appendOutput();
    QList<FileEntry> m_files;
    QSet<int> m_selected;
    QString m_diagnostic, m_summary, m_taskTitle, m_taskOutput;
    bool m_scanning = false;
    QProcess m_process;
    QFutureWatcher<ScanResult> m_watcher;
};
