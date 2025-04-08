#include <QApplication>
#include "mainwindow.h"
#include "reconnection.h"
#include "regwindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Reconnection& reconnection = Reconnection::getInterface();
    RegWindow* regwindow = new RegWindow();

    reconnection.setRegwind(regwindow);
    regwindow->show();        

    int regwindresult = app.exec();

    if (regwindresult == 0) {
        regwindow->close();
        delete regwindow;
        MainWindow mainwindow;
        mainwindow.show();
        return app.exec();
    }

    delete regwindow;
    return regwindresult;
}
