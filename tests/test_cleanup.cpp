#include "cleanup.h"
#include <QFile>
#include <QDir>
#include <QTemporaryDir>
#include <QtTest>

class CleanupTest : public QObject {
    Q_OBJECT
private slots:
    void scanAndClean() {
        QTemporaryDir dir; QVERIFY(dir.isValid());
        const auto now = QDateTime::currentDateTimeUtc();
        auto makeOld = [&](const QString &name) {
            QFile f(dir.filePath(name)); if (!f.open(QIODevice::WriteOnly)) return false;
            f.write("old"); f.flush(); return f.setFileTime(now.addDays(-8), QFileDevice::FileModificationTime);
        };
        QVERIFY(makeOld("old")); QVERIFY(makeOld("changed"));
        QFile recent(dir.filePath("recent")); QVERIFY(recent.open(QIODevice::WriteOnly)); recent.write("recent"); recent.close();
        auto list = scanFiles(dir.path(), now); QCOMPARE(list.size(), 2);
        QFile changed(dir.filePath("changed")); QVERIFY(changed.open(QIODevice::Append)); changed.write("new"); changed.close();
        auto result = cleanFiles(dir.path(), list, now);
        QCOMPARE(result.removed, 1); QCOMPARE(result.skipped, 1);
        QVERIFY(!QFile::exists(dir.filePath("old"))); QVERIFY(QFile::exists(dir.filePath("changed"))); QVERIFY(QFile::exists(dir.filePath("recent")));
    }
    void recursiveScanAndLimit() {
        QTemporaryDir dir; QVERIFY(dir.isValid());
        QVERIFY(QDir(dir.path()).mkdir("nested"));
        const auto now = QDateTime::currentDateTimeUtc();
        for (const auto &name : {QStringLiteral("one"), QStringLiteral("nested/two")}) {
            QFile f(dir.filePath(name)); QVERIFY(f.open(QIODevice::WriteOnly));
            f.write("data"); QVERIFY(f.flush()); QVERIFY(f.setFileTime(now.addDays(-8), QFileDevice::FileModificationTime)); f.close();
        }
        const auto scan = scanRecursive(dir.path(), 10, now);
        QCOMPARE(scan.files.size(), 2); QVERIFY(!scan.truncated);
        const auto limited = scanRecursive(dir.path(), 1, now);
        QCOMPARE(limited.files.size(), 1); QVERIFY(limited.truncated);
        const auto result = cleanFiles(dir.path(), {scan.files.last()}, now);
        QCOMPARE(result.removed, 1); QCOMPARE(result.skipped, 0);
    }
    void outsidePath() {
        QTemporaryDir a, b; QVERIFY(a.isValid()); QVERIFY(b.isValid());
        QFile f(b.filePath("outside")); QVERIFY(f.open(QIODevice::WriteOnly)); f.write("x"); f.flush();
        const auto now = QDateTime::currentDateTimeUtc(); QVERIFY(f.setFileTime(now.addDays(-8), QFileDevice::FileModificationTime)); f.close();
        auto list = scanFiles(b.path(), now); QCOMPARE(list.size(), 1);
        auto result = cleanFiles(a.path(), list, now); QCOMPARE(result.removed, 0); QCOMPARE(result.skipped, 1); QVERIFY(f.exists());
    }
};
QTEST_GUILESS_MAIN(CleanupTest)
#include "test_cleanup.moc"
