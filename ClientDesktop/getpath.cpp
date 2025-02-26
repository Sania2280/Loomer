#include "getpath.h"
#include <QDir>
#include <QCoreApplication>

Get_My_Path::Get_My_Path() {}


QString Get_My_Path::GetPath(Directorys target, Files directory) {
    QString path;
    QString file;
    QString extention;

    if (target == Directorys::IMAGED) {
        path = "images";
        extention = ".png";
    }
    if (target == Directorys::STYLES) {
        path = "styles";
        extention = ".qss";
    }
    if (target == Directorys::CONFIG) {
        file = "config";
        extention = ".json";
    }
    if (directory == Files::USER) {
        file = "user";
    }
    if (directory == Files::BUTON) {
        file = "send";
    }
    if (directory == Files::STYLE) {
        file = "style";
    }
    if (directory == Files::ITEM) {
        file = "item_style";
    }

    QString appDir = QCoreApplication::applicationDirPath();
    QDir baseDir(appDir);
    baseDir.cdUp(); // Поднимаемся к папке build
    baseDir.cdUp(); // Поднимаемся к папке Client
    baseDir.cdUp();// Поднимаемся к папке Client
    if (directory != Files::NON)
        baseDir.cd(path);

    QString filePath = baseDir.filePath(file + extention);
    qDebug() << "File path:" << filePath;

    return filePath;
}
