#include "cleanup.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>

static constexpr qint64 minimumAge = 7 * 24 * 60 * 60;
QString humanSize(qint64 size) {
    double n = size;
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    while (n >= 1024 && i < 4) { n /= 1024; ++i; }
    return QString::number(n, 'f', 1) + QLatin1Char(' ') + QLatin1String(units[i]);
}
QList<FileEntry> scanFiles(const QString &root, const QDateTime &now) {
    QList<FileEntry> result;
    QDir dir(root);
    for (const auto &info : dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDir::Name)) {
        if (!info.isSymLink() && info.isFile() && info.lastModified().secsTo(now) > minimumAge)
            result.append({info.absoluteFilePath(), info.size(), info.lastModified()});
    }
    return result;
}
CleanupResult cleanFiles(const QString &root, const QList<FileEntry> &files, const QDateTime &now) {
    CleanupResult result;
    const QString canonicalRoot = QFileInfo(root).canonicalFilePath();
    for (const auto &entry : files) {
        QFileInfo info(entry.path);
        if (canonicalRoot.isEmpty() || info.dir().canonicalPath() != canonicalRoot || info.isSymLink() || !info.isFile()
            || info.size() != entry.size || info.lastModified() != entry.modified || info.lastModified().secsTo(now) <= minimumAge) {
            ++result.skipped; continue;
        }
        if (QFile::remove(entry.path)) ++result.removed; else ++result.skipped;
    }
    return result;
}
