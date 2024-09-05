#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    if (socket->state() != QAbstractSocket::ConnectedState) {
        socket->connectToHost("127.0.0.1", 2323);
    }

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::slotReadyRead() {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_0);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;


        if(str.startsWith("thEinDeNtIfIcAtOr")){
            QString identifierPart = str.mid(QString("thEinDeNtIfIcAtOr ").length());
            QStringList parts = identifierPart.split(", ");
            if (parts.size() == 2) {
                if(ui->listWidget->findItems(parts[1],Qt::MatchExactly).isEmpty()){
                    QString string = parts[0]+" | "+parts[1];
                ui->listWidget->addItem(parts[1]);
                    qDebug()<<"Add";
                parts.clear();
                }
            }
        }

         if (str.startsWith("mYthEinDeNtIfIcAtOr")){
            QString identifierPart = str.mid(QString("thEinDeNtIfIcAtOr ").length());
            QStringList parts = identifierPart.split(", ");
            QString num = parts[1];
            setWindowTitle(num);
        }

         if (str.startsWith("tOrEmUvE ")) {
             qDebug() << "Delete request received";
             QString identifierPart = str.mid(QString("tOrEmUvE ").length());
             QStringList parts = identifierPart.split(", ");

             if (parts.size() > 1) {
                 QString valueToRemove = parts[1];  // Значення, яке потрібно знайти та видалити
                 QList<QListWidgetItem*> items = ui->listWidget->findItems(valueToRemove, Qt::MatchExactly);

                 if (!items.isEmpty()) {
                     // Видаляємо всі знайдені елементи
                     for (QListWidgetItem* item : items) {
                         int row = ui->listWidget->row(item);
                         delete ui->listWidget->takeItem(row);
                     }
                     qDebug() << "Item with value" << valueToRemove << "deleted";
                 } else {
                     qDebug() << "Error: Item with value" << valueToRemove << "not found";
                 }
             } else {
                 qDebug() << "Error: Insufficient parts in the identifier";
             }
         }




    }

    else {
        ui->textBrowser->append("Read error");
    }
}


void MainWindow::SendToServer(const QString& str) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << str;
        socket->write(data);
        this->setWindowTitle(data);
    }
    else {
        ui->textBrowser->append("Socket not connected");
    }
}


void MainWindow::on_pushButton_2_clicked() {
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_returnPressed() {
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString num = item->text();
    ui->textBrowser->append(num);
}

