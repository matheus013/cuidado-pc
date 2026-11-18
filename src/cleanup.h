#pragma once
#include <QList>
#include <QString>
#include <QDateTime>

struct FileEntry { QString path; qint64 size; QDateTime modified; };
struct CleanupResult { int removed = 0; int skipped = 0; };
struct ScanResult { QList<FileEntry> files; bool truncated = false; QString error; };
QList<FileEntry> scanFiles(const QString &root, const QDateTime &now = QDateTime::currentDateTimeUtc());
ScanResult scanRecursive(const QString &root, int limit = 10000, const QDateTime &now = QDateTime::currentDateTimeUtc());
CleanupResult cleanFiles(const QString &root, const QList<FileEntry> &files, const QDateTime &now = QDateTime::currentDateTimeUtc());
QString humanSize(qint64 size);
