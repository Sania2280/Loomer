#include <QApplication>
#include "mainwindow.h"
#include "RegWindow.h"
#include "UserData.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    // UserData& userdata = UserData::getInstance();

    RegWindow* regwindow = new RegWindow();
    regwindow->show();

    int regwindresult = app.exec();


    if (regwindresult == 0) {
        delete regwindow;
        MainWindow mainwindow;
        mainwindow.show();
        return app.exec();
    }

    delete regwindow;
    return regwindresult;
}
