#include <QCoreApplication>

#include "server.h"
#include "sending.h"
#include "Config.hpp"

#include <QFile>

static void myLogMessageHandler(const QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString formattedMsg = qFormatLogMessage(type, context, msg) + "\n";

    // Записываем в файл
    QFile logFile("app.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        logFile.write(qUtf8Printable(formattedMsg));
    }

    // Выводим в консоль
    fprintf(stderr, "%s", qUtf8Printable(formattedMsg));
    fflush(stderr);
}

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    qInstallMessageHandler(myLogMessageHandler);
    // Install custom message pattern
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss,zzz} [%{type}] [%{line}] [%{file}]: %{message}");

    Config config{"./config.json"};
    config.Load();

    server server{config.GetSettings()};

    Sending sendObj(&server);

    sendObj.start();

    server.setSending(sendObj);

    return a.exec();
}
