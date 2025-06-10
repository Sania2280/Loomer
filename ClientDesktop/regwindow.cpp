#include "regwindow.h"
#include "ui_RegWindow.h"
#include "UserData.h"
#include "enums.h"
#include "datasender.h"



#include <QFile>
#include <qtimer.h>
#include <QKeyEvent>


DataSender  dataSendler;
UserData& userData = UserData::getInstance();


RegWindow::RegWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::regwindow)
    , dataSendler(new DataSender(nullptr))

{

    ui->setupUi(this);
    this->setStyleSheet(Style_Sheete());
    QIcon retrun_buton_icon("./images/Return_buton.png");

    ui->pushButton_return->setIcon(retrun_buton_icon);
    ui->label->setText("Log In in your account or create new one");
    ui->lineEdit_name->setPlaceholderText("Write name...");
    ui->lineEdit_pass->setPlaceholderText("Write pass...");

    StartUpWindow();

    // serverConnector->ConnectToServer();

    connect(this, &::RegWindow::CloseWindow, this,[this](){

        userData.mainWindStarted = true;
        this->close();
        qDebug() << "Go to main";
    });

}

RegWindow::~RegWindow()
{
    delete ui;
    delete dataSendler;
}


void RegWindow::StartUpWindow()
{
    ui->label->show();
    ui->pushButton_Log_in->show();
    ui->pushButton_Sing_up->show();


    ui->lineEdit_name->hide();
    ui->lineEdit_pass->hide();
    ui->pushButton_end->hide();
    ui->pushButton_return->hide();

    LogInWindStat = false;
    SingUpWindStat = false;
}

void RegWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event";
    QDialog::closeEvent(event);
    CloseWindow();
}


QString RegWindow::Style_Sheete() {

    QString res = "" ;

    QFile styleFile("./styles/regstyle.qss"); // Убедитесь, что путь корректный
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        styleFile.close();
        res = styleSheet;
    }
    else qDebug() << "Style File not open";

    return res;
}

void RegWindow::on_pushButton_Log_in_clicked()
{
    userData.status = RegWind::LOG;
    ShowMainForm();
    ui->pushButton_end->setText("Log In");

    LogInWindStat = true;
}

void RegWindow::on_pushButton_Sing_up_clicked()
{
    userData.status = RegWind::SIGN;
    ShowMainForm();
    ui->pushButton_end->setText("Sing Up");

    SingUpWindStat = true;
}


void RegWindow::ShowMainForm()
{
    ui->lineEdit_name->show();
    ui->lineEdit_pass->show();
    ui->pushButton_end->show();
    ui->pushButton_return->show();


    ui->pushButton_Log_in->hide();
    ui->pushButton_Sing_up->hide();

}


void RegWindow::on_pushButton_return_clicked()
{
    StartUpWindow();
    ui->listWidget_errors->clear();
}


void RegWindow::on_pushButton_end_clicked()
{

    userData.name = ui->lineEdit_name->text();
    userData.pass = ui->lineEdit_pass->text();


    if(userData.status == RegWind::LOG) {
        dataSendler->SendMyData(MesageIdentifiers::LOG);
    }
    else if (userData.status == RegWind::SIGN) {
        dataSendler->SendMyData(MesageIdentifiers::SIGN);
    }

}

void RegWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        if(LogInWindStat == true || SingUpWindStat == true){
            RegWindow::on_pushButton_end_clicked();
        }
    }

    if(event->key() == Qt::Key_Up){
        if(LogInWindStat == true || SingUpWindStat == true){
            ui->lineEdit_name->setFocus();
        }
    }

    if(event->key() == Qt::Key_Down){
        if(LogInWindStat == true || SingUpWindStat == true){
            ui->lineEdit_pass->setFocus();
        }
    }
}

void RegWindow::logSingWorker(MesageIdentifiers id)
{
    qDebug() << "errorWorker";
   if (id == MesageIdentifiers::LOGIN_FAIL_NAME)
    {
        qDebug() << "Log in fail NAME";
        ui->listWidget_errors->clear();
        ui->listWidget_errors->addItem("ERROR: Incorrect Name");
    }

    else if (id == MesageIdentifiers::SIGN_FAIL)
    {
        qDebug() << "Account creation ERROR";
        ui->listWidget_errors->clear();
        ui->listWidget_errors->addItem("ERROR: Account creation Failed");
    }
    else if (id == MesageIdentifiers::SIGN_FAIL_EXIST)
    {
        qDebug() << "User exists ERROR";
        ui->listWidget_errors->clear();
        ui->listWidget_errors->addItem("ERROR: User already exists");
    }
    else if (id == MesageIdentifiers::LOGIN_FAIL_PASS)
    {
        qDebug() << "Log in fail PASS";
        ui->listWidget_errors->clear();
        ui->listWidget_errors->addItem("ERROR: Incorrect Password");
    }
}

void RegWindow::connectStatWorker(ConnectionStat stat)
{
    if(stat == ConnectionStat::CONNECTED){
        ui->listWidget_errors->clear();
        ui->listWidget_errors->addItem("Connected to Server");
        QTimer::singleShot(3000, ui->listWidget_errors, &QListWidget::clear);
    }
    else if (stat == ConnectionStat::DISCONNECTED){
        ui->listWidget_errors->clear();
        ui->listWidget_errors->addItem("ERROR: lost connection");
    }
}


