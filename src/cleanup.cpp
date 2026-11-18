#include "cleanup.h"
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>

static constexpr qint64 minimumAge = 7 * 24 * 60 * 60;
QString humanSize(qint64 size) {
    double n = size;
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0; while (n >= 1024 && i < 4) { n /= 1024; ++i; }
    return QString::number(n, 'f', 1) + QLatin1Char(' ') + QLatin1String(units[i]);
}
QList<FileEntry> scanFiles(const QString &root, const QDateTime &now) {
    QList<FileEntry> result;
    for (const auto &info : QDir(root).entryInfoList(QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDir::Name))
        if (!info.isSymLink() && info.isFile() && info.lastModified().secsTo(now) > minimumAge)
            result.append({info.absoluteFilePath(), info.size(), info.lastModified()});
    return result;
}
static bool withinRoot(const QString &root, const QFileInfo &info) {
    const auto canonicalRoot = QFileInfo(root).canonicalFilePath();
    const auto canonicalParent = info.dir().canonicalPath();
    if (canonicalRoot.isEmpty() || canonicalParent.isEmpty()) return false;
    const auto prefix = canonicalRoot.endsWith('/') ? canonicalRoot : canonicalRoot + '/';
    return canonicalParent == canonicalRoot || canonicalParent.startsWith(prefix);
}
ScanResult scanRecursive(const QString &root, int limit, const QDateTime &now) {
    ScanResult result;
    QFileInfo rootInfo(root);
    if (!rootInfo.isDir() || !rootInfo.isReadable()) { result.error = QStringLiteral("Não foi possível acessar a pasta temporária."); return result; }
    QDirIterator it(root, QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next(); const QFileInfo info = it.fileInfo();
        if (info.isSymLink() || !info.isFile() || !withinRoot(root, info) || info.lastModified().secsTo(now) <= minimumAge) continue;
        if (result.files.size() >= limit) { result.truncated = true; break; }
        result.files.append({info.absoluteFilePath(), info.size(), info.lastModified()});
    }
    return result;
}
CleanupResult cleanFiles(const QString &root, const QList<FileEntry> &files, const QDateTime &now) {
    CleanupResult result;
    for (const auto &entry : files) {
        QFileInfo info(entry.path);
        if (!withinRoot(root, info) || info.isSymLink() || !info.isFile() || info.size() != entry.size || info.lastModified() != entry.modified || info.lastModified().secsTo(now) <= minimumAge) { ++result.skipped; continue; }
        if (QFile::remove(entry.path)) ++result.removed; else ++result.skipped;
    }
    return result;
}
