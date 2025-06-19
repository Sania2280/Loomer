#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

#include "customwidgetitem.h"
#include "getpath.h"
#include "UserData.h"
#include "message.h"
#include "datasender.h"
#include "clientdatabase.h"

#include <QListWidget>
#include <QStringBuilder>
#include <QDir>
#include <QTimer>


#include "enums.h"

Config::Settings Config::settings;
UserData& userdata = UserData::getInstance();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {


    qDebug() << "User name:" << userdata.name;
    this->socket = userdata.getSocket();
    MySocket = userdata.desck;


    ui->setupUi(this);
    this->setStyleSheet(Style_Sheete());

    setWindowTitle(userdata.name + " / " + MySocket + " / " + userdata.id);

    QIcon buton_icon("./images/send.png");
    ui->pushButton->setIcon(buton_icon);

    ui->listWidget_2->setSpacing(7);


    Config config;
    config.Read();

    qDebug() << QCoreApplication::applicationDirPath();

    qDebug() << "Sock state: " << socket->state();

    Message messToSend;

    messToSend.id = MesageIdentifiers::CLIENT_READY_TO_WORCK;
    messToSend.dbID = userdata.id.toStdString();
    messToSend.messageData.senderDesk = MySocket.toStdString();
    qDebug() <<  "Good id: "<<messToSend.dbID;

    SendToServer(messToSend);


}

void MainWindow::closeEvent(QCloseEvent *event){
    Close_Window_stat = true;
    socket->abort();
    QMainWindow::closeEvent(event);

}


MainWindow::~MainWindow() {
    delete ui;
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}


void MainWindow::SendToServer(Message &message) {
    DataSender dataSendler;
    dataSendler.SendMyMasseg(message);

}

void MainWindow::on_lineEdit_returnPressed() {
    if(!Interlocutor.isEmpty() && ui->lineEdit->text() != QString()){

            Message message;
            message.id = MesageIdentifiers::MESAGE;
            message.messageData.resivDesk = Interlocutor.toStdString();
            message.messageData.senderDesk = MySocket.toStdString();
            message.messageData.message = ui->lineEdit->text().toStdString();


            CustomListItem *widget = new CustomListItem(ui->lineEdit->text());
            QWidget *container = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout(container);

            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);

            layout->addStretch();

            layout->addWidget(widget);

            container->setLayout(layout);

            // Создаём элемент списка и устанавливаем контейнер как виджет элемента
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_2);
            item->setSizeHint(container->sizeHint());
            ui->listWidget_2->setItemWidget(item, container);
            ui->listWidget_2->scrollToBottom();


            SendToServer(message);

        }
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_clicked() { on_lineEdit_returnPressed(); }



void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString num = item->text();
    item->setBackground(QBrush(QColor(QColorConstants::Svg::lightblue)));
    Interlocutor = num;
    ui->lineEdit->setFocus();

    for(int i = 0; i < ui->listWidget->count(); i++){
        QListWidgetItem *itemLocal = ui->listWidget->item(i);
        if(itemLocal->text() == num)itemLocal->setBackground(QBrush(QColor(QColorConstants::Svg::lightblue)));
        else itemLocal->setBackground(Qt::NoBrush); // Сбрасывает цвет фона
    }
}


void MainWindow::Socket_print() {
    for (const auto &i : userdata.Sockets) {
        QList<QListWidgetItem *> item =
            ui->listWidget->findItems(i, Qt::MatchExactly);
        if (item.isEmpty()) {
            QListWidgetItem *itemlocal =
                new QListWidgetItem(QIcon("./images/user.png"), i);
            ui->listWidget->setIconSize(QSize(25, 25));
            ui->listWidget->addItem(itemlocal);
        }
    }
}

void MainWindow::Socket_delete(QString socket_to_delete) {
    qDebug() << "Soc to Del" << socket_to_delete;

    QList<QListWidgetItem *> items =
        ui->listWidget->findItems(socket_to_delete, Qt::MatchExactly);
    if (!items.isEmpty()) {
        for (QListWidgetItem *item : items) {
            int row = ui->listWidget->row(item); // Получить строку элемента
            delete ui->listWidget->takeItem(row);
        }
    }
}

void MainWindow::PrintMassage(QString massage)
{
    qDebug() << "MESAGE: " << massage;

    CustomListItem *widget = new CustomListItem(massage);
    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(container);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(widget);

    container->setLayout(layout);
    layout->addStretch();

    QListWidgetItem *item = new QListWidgetItem(ui->listWidget_2);
    item->setSizeHint(container->sizeHint());
    ui->listWidget_2->setItemWidget(item, container);
    ui->listWidget_2->scrollToBottom();
}


QString MainWindow::Style_Sheete() {

    QString res = "" ;

    Get_My_Path get_my_path;

    QFile styleFile("./styles/style.qss"); // Убедитесь, что путь корректный
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        styleFile.close();
        res = styleSheet;

    }
    else qDebug() << "Style File not open";

    return res;
}
