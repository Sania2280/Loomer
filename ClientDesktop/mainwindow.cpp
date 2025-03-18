#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "m_pack.h"
#include "customwidgetitem.h"
#include "getpath.h"
#include "UserData.h"
#include "message.h"

#include <QListWidget>
#include <QStringBuilder>
#include <QDir>
#include <QTimer>



#include <msgpack.hpp>

#include "enums.h"


Config::Settings Config::settings;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    UserData& userdata = UserData::getInstance();

    qDebug() << "User name:" << userdata.name;

    this->socket = userdata.getSocket();

    qDebug()<<"Socket on MianWind" << socket << "desck" << socket->socketDescriptor();

    qDebug() << "resived" << userdata.desck;

    MySocket = userdata.desck;


    ui->setupUi(this);
    this->setStyleSheet(Style_Sheete());

    setWindowTitle(userdata.name + " / " + MySocket);

    QIcon buton_icon("./images/send.png");
    ui->pushButton->setIcon(buton_icon);

    ui->listWidget_2->setSpacing(7);


    Config config;
    config.Read();

    setupConnection();

    qDebug() << QCoreApplication::applicationDirPath();

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);

    Message messToSend;

    messToSend.id = MesageIdentifiers::CLIENT_READY_TO_WORCK;
    messToSend.messageData.senderDesk = MySocket.toStdString();

    // QString message = QString("%1,%2")
    //                       .arg(CLIENT_READY_TO_WORCK)
    //                       .arg(MySocket);

    // SendToServer(message);

}

void MainWindow::closeEvent(QCloseEvent *event){
    Close_Window_stat = true;
    socket->abort();
    QMainWindow::closeEvent(event);

}

void MainWindow::setupConnection(){
    // connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    // connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onError);
    // connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    // socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);

}


void MainWindow::onConnected() {
    qDebug() << "Connected to Server";
}

void MainWindow::onError(QAbstractSocket::SocketError error) {
    if(Close_Window_stat)return;

    qWarning() << "Error connect to Server:" << socket->errorString();
    // Запускаем повторное подключение через 3 секунды:
    QTimer::singleShot(3000, this, &MainWindow::setupConnection);
}

void MainWindow::onDisconnected() {
     if(Close_Window_stat)return;

    qWarning() << "Disconnected from Server. Reconnecting...";
    QTimer::singleShot(3000, this, &MainWindow::setupConnection);
}


MainWindow::~MainWindow() {
    delete ui;
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void MainWindow::slotReadyRead() {
    M_pack msg_p;
    Message message = msg_p.unpack(socket->readAll().toStdString());

    switch (message.id) {

        case MesageIdentifiers::ID_CLIENT: // the_identifier
        {
            if (!Sockets.contains(QString::fromStdString(message.newOrDeleteClientInNet.descriptor))){
                Sockets.push_back(QString::fromStdString(message.newOrDeleteClientInNet.descriptor));
                MainWindow::Socket_print();
            }
            break;
        }

        case MesageIdentifiers::ID_DELETE: // delete_client;
        {
            Sockets.removeAll(message.newOrDeleteClientInNet.descriptor);
            MainWindow::Socket_delete(QString::fromStdString(message.newOrDeleteClientInNet.descriptor));
            break;
        }

        case MesageIdentifiers::MESAGE: // message
        {
            CustomListItem *widget = new CustomListItem(QString::fromStdString(message.messageData.message));
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
            break;
        }
    }

}

void MainWindow::SendToServer(Message &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        M_pack m_pack;
        socket->write(m_pack.puck(message).data());
        qDebug() << socket;
    } else {
        qDebug() << "Socket not connected";
    }
}

void MainWindow::on_lineEdit_returnPressed() {
    if(!Interlocutor.isEmpty() && ui->lineEdit->text() != QString()){
    UserData& userdata = UserData::getInstance();

    // QString message = QString("%1,%2,%3,%4")
    // .arg(MESAGE)
    //     .arg(Interlocutor)
    //     .arg(MySocket)
    //     .arg(ui->lineEdit->text());

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
        QListWidgetItem *item = ui->listWidget->item(i);
        if(item->text() == num)item->setBackground(QBrush(QColor(QColorConstants::Svg::lightblue)));
        else item->setBackground(Qt::NoBrush); // Сбрасывает цвет фона
    }
}


void MainWindow::Socket_print() {
    for (auto i : Sockets) {
        QList<QListWidgetItem *> item =
            ui->listWidget->findItems(i, Qt::MatchExactly);
        if (item.isEmpty()) {
            QListWidgetItem *item =
                new QListWidgetItem(QIcon("./images/user.png"), i);
            ui->listWidget->setIconSize(QSize(25, 25));
            ui->listWidget->addItem(item);
        }
    }
}

void MainWindow::Socket_delete(QString socket_to_delete) {

    QList<QListWidgetItem *> items =
        ui->listWidget->findItems(socket_to_delete, Qt::MatchExactly);
    if (!items.isEmpty()) {
        for (QListWidgetItem *item : items) {
            int row = ui->listWidget->row(item); // Получить строку элемента
            delete ui->listWidget->takeItem(row);
        }
    }
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



