#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QListWidget>
#include<QStringBuilder>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    if (socket->state() != QAbstractSocket::ConnectedState) {
        socket->connectToHost("192.168.186.1", 2323);
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

        qDebug()<<str;

        QString Identifier = str.left(2);
        int messType = Identifier.toInt();
        if(messType==04){
        ui->textBrowser->append(Identifier);
        }

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
                // ui->listWidget->clear();
            }

        }

         if (str.startsWith("mYthEinDeNtIfIcAtOr")){
            QString identifierPart = str.mid(QString("myEinDeNtIfIcAtOr ").length());
            QStringList parts = identifierPart.split(", ");
            QString num = parts[1];
            setWindowTitle(num);
            MySocket =/* parts[0] % " " %*/ parts[1];
            qDebug()<<"My socket"<<MySocket;
        }

         if (str.startsWith("tOrEmUvE ")) {
             qDebug() << "Delete request received";
             QString identifierPart = str.mid(QString("tOrEmUvE ").length());
             QStringList parts = identifierPart.split(", ");

             if (parts.size() > 1) {
                 QString valueToRemove = parts[1];  // Значення, яке потрібно знайти та видалити
                 qDebug()<<"To delete" << valueToRemove;
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

         if(str.startsWith("AlOuSeComMuniCaTiOn ")){
             QString identifier = str.mid(QString("AlOuSeComMuniCaTiOn ,").length()).trimmed();
             ActiveDialog.push_back(identifier);
             qDebug()<<identifier << "reciver";
         }

         if(str.startsWith("MeSaGe ")){

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
        qDebug()<<socket;
        // this->setWindowTitle(data);
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
    // Формируем строку сообщения
    QString message = QString("04,%1,%2,%3")
                          .arg(Interlocutor)
                          .arg(MySocket)
                          .arg(ui->lineEdit->text());


    SendToServer(message);

    ui->lineEdit->clear();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString num = item->text();
    QString socket = ("hWaNtToTaLk ") % MySocket % ", " % num;
    ui->textBrowser->append(num);
    qDebug()<<socket;
   // SendToServer(socket);
    Interlocutor = num;
    delete item;
}


void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{

}

void MainWindow::closeEvent(QCloseEvent* event) {
    // Пример: Сохраняем данные перед закрытием
    bool shouldSave = true;  // Эта переменная может быть результатом вашего алгоритма сохранения
    if (shouldSave) {
        // Сохраняем данные
        qDebug() << "Saving data...";
    }
}
