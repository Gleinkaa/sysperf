#include "config.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QStringList>

namespace sysperf {

namespace {

std::string toStd(const QString& s) { return s.toStdString(); }

// Find the config file: $SYSPERF_CONFIG, then --config <path>, then ./config/default.json,
// then <exe-dir>/config/default.json, then <exe-dir>/../config/default.json.
QString resolveConfigPath(const QStringList& args) {
    QByteArray env = qgetenv("SYSPERF_CONFIG");
    if (!env.isEmpty()) return QString::fromLocal8Bit(env);

    for (int i = 0; i < args.size() - 1; ++i)
        if (args[i] == "--config") return args[i + 1];

    QStringList candidates;
    candidates << QDir::current().filePath("config/default.json");
    const QString exeDir = QCoreApplication::applicationDirPath();
    if (!exeDir.isEmpty()) {
        candidates << QDir(exeDir).filePath("config/default.json");
        candidates << QDir(exeDir).filePath("../config/default.json");
    }
    for (const QString& c : candidates)
        if (QFileInfo::exists(c)) return c;
    return {};
}

void applyJson(AppConfig& cfg, const QJsonObject& root) {
    const QJsonObject col = root.value("collector").toObject();
    if (col.contains("interval_ms")) cfg.interval_ms = col.value("interval_ms").toInt(cfg.interval_ms);
    if (col.contains("history_seconds"))
        cfg.history_seconds = col.value("history_seconds").toInt(cfg.history_seconds);
    if (col.contains("proc_root"))
        cfg.proc_root = toStd(col.value("proc_root").toString(QString::fromStdString(cfg.proc_root)));
    if (col.contains("sys_root"))
        cfg.sys_root = toStd(col.value("sys_root").toString(QString::fromStdString(cfg.sys_root)));

    const QJsonObject bud = root.value("budget").toObject();
    if (bud.contains("pss_mb_warn")) cfg.pss_mb_warn = bud.value("pss_mb_warn").toDouble(cfg.pss_mb_warn);
    if (bud.contains("cpu_pct_warn")) cfg.cpu_pct_warn = bud.value("cpu_pct_warn").toDouble(cfg.cpu_pct_warn);

    const QJsonObject ui = root.value("ui").toObject();
    if (ui.contains("start_panel"))
        cfg.start_panel = toStd(ui.value("start_panel").toString(QString::fromStdString(cfg.start_panel)));

    const QJsonObject log = root.value("logging").toObject();
    if (log.contains("level"))
        cfg.log_level = toStd(log.value("level").toString(QString::fromStdString(cfg.log_level)));
}

void applyEnv(AppConfig& cfg) {
    auto envStr = [](const char* k, std::string& dst) {
        QByteArray v = qgetenv(k);
        if (!v.isEmpty()) dst = std::string(v.constData(), static_cast<std::size_t>(v.size()));
    };
    envStr("SYSPERF_PROC_ROOT", cfg.proc_root);
    envStr("SYSPERF_SYS_ROOT", cfg.sys_root);
    QByteArray iv = qgetenv("SYSPERF_INTERVAL_MS");
    if (!iv.isEmpty()) {
        bool ok = false;
        int n = QString::fromLocal8Bit(iv).toInt(&ok);
        if (ok && n > 0) cfg.interval_ms = n;
    }
}

void applyCli(AppConfig& cfg, const QStringList& args) {
    for (int i = 0; i < args.size(); ++i) {
        const QString& a = args[i];
        auto next = [&](std::string& dst) {
            if (i + 1 < args.size()) dst = toStd(args[++i]);
        };
        if (a == "--proc-root") next(cfg.proc_root);
        else if (a == "--sys-root") next(cfg.sys_root);
        else if (a == "--interval-ms" && i + 1 < args.size()) {
            bool ok = false;
            int n = args[++i].toInt(&ok);
            if (ok && n > 0) cfg.interval_ms = n;
        }
    }
}

} // namespace

AppConfig AppConfig::load(const QStringList& args) {
    AppConfig cfg;  // compiled defaults

    const QString path = resolveConfigPath(args);
    if (!path.isEmpty()) {
        QFile f(path);
        if (f.open(QIODevice::ReadOnly)) {
            QJsonParseError err{};
            const QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
            if (err.error == QJsonParseError::NoError && doc.isObject()) {
                applyJson(cfg, doc.object());
            } else {
                qWarning("sysperf: ignoring malformed config %s: %s",
                         qUtf8Printable(path), qUtf8Printable(err.errorString()));
            }
        }
    }

    applyEnv(cfg);
    applyCli(cfg, args);

    if (cfg.interval_ms < 100) cfg.interval_ms = 100;            // floor: don't busy-spin
    if (cfg.history_seconds < 10) cfg.history_seconds = 10;
    return cfg;
}

} // namespace sysperf
