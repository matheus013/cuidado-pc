#pragma once
#include "cleanup.h"
#include <QObject>
#include <QVariantList>

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString diagnostic READ diagnostic NOTIFY changed)
    Q_PROPERTY(QString summary READ summary NOTIFY changed)
    Q_PROPERTY(QVariantList files READ files NOTIFY changed)
    Q_PROPERTY(int count READ count NOTIFY changed)
public:
    explicit Backend(QObject *parent = nullptr);
    QString diagnostic() const { return m_diagnostic; }
    QString summary() const { return m_summary; }
    QVariantList files() const;
    int count() const { return int(m_files.size()); }
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void scan();
    Q_INVOKABLE void clean();
signals:
    void changed();
private:
    QList<FileEntry> m_files;
    QString m_diagnostic, m_summary;
};
