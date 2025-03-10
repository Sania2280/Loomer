#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

#include "Mpack.hpp"

#include "customwidgetitem.h"
#include "getpath.h"
#include "UserData.h"

#include <QListWidget>
#include <QStringBuilder>
#include <QDir>
#include <QTimer>

#include "Mpack.hpp"

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

    setWindowTitle(MySocket + " / " + userdata.name);

    QIcon buton_icon("./images/send.png");
    ui->pushButton->setIcon(buton_icon);

    ui->listWidget_2->setSpacing(7);


    Config config;
    config.Read();

    setupConnection();

    qDebug() << QCoreApplication::applicationDirPath();

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);

    QString message = QString("%1,%2")
                          .arg(CLIENT_READY_TO_WORCK)
                          .arg(MySocket);

    SendToServer(message);

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

void MainWindow::onError(QAbstractSocket::SocketError /*error*/) {
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
    const QString str = Mpack::unpack(socket->readAll());
    QStringList parts = str.split(",");
    int messType = parts[0].toInt();

    switch (messType) {

        case ID_MY: // my_identifier
        {
            // MySocket = parts[2];
            QString num = parts[2];
            setWindowTitle(num);
            qDebug() << "My socket" << MySocket;
            break;
        }

        case ID_CLIENT: // the_identifier
        {
            if (!Sockets.contains(parts[2])) {
                Sockets.push_back(parts[2]);
                MainWindow::Socket_print();
            }

            parts.clear();
            break;
        }

        case ID_DELETE: // delete_client;
        {
            Sockets.removeAll(parts[2]);
            MainWindow::Socket_delete(parts[2]);
            break;
        }

        case MESAGE: // message
        {
            CustomListItem *widget = new CustomListItem(parts[3]);
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
        default:
        {
            qFatal() << "void MainWindow::slotReadyRead. unknow messType";
        }
    }

}

void MainWindow::SendToServer(const QString &str) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(Mpack::puck(str).data());
        qDebug() << socket;
    } else {
        qDebug() << "Socket not connected";
    }
}

void MainWindow::on_lineEdit_returnPressed() {
    if(!Interlocutor.isEmpty() && ui->lineEdit->text() != QString()){
    // UserData& userdata = UserData::getInstance();

    QString message = QString("%1,%2,%3,%4")
    .arg(MESAGE)
        .arg(Interlocutor)
        .arg(MySocket)
        .arg(ui->lineEdit->text());

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
    for (auto i : Sockets) {
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
