#pragma once
#include <QList>
#include <QString>
#include <QDateTime>

struct FileEntry { QString path; qint64 size; QDateTime modified; };
struct CleanupResult { int removed = 0; int skipped = 0; };
QList<FileEntry> scanFiles(const QString &root, const QDateTime &now = QDateTime::currentDateTimeUtc());
CleanupResult cleanFiles(const QString &root, const QList<FileEntry> &files, const QDateTime &now = QDateTime::currentDateTimeUtc());
QString humanSize(qint64 size);
