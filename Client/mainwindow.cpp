#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>
#include <QStringBuilder>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringBuilder>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    if (socket->state() != QAbstractSocket::ConnectedState) {
        socket->connectToHost("192.168.1.168", 2323);
    }

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);


}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::slotReadyRead() {
    QDataStream in(socket);

    in.setVersion(QDataStream::Qt_6_0);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;

        QString Identifier = str.left(2);
        int messType = Identifier.toInt();
        QStringList parts = str.split(",");

        switch (messType) {

            case 02: // my_identifier
            {
                MySocket = parts[2];
                QString num = parts[2];
                setWindowTitle(num);
                qDebug() << "My socket" << MySocket;
                break;
            }

            case 03: // the_identifier
            {
                if (!Sockets.contains(parts[2])) {
                    Sockets.push_back(parts[2]);
                    MainWindow::Socket_print();
                }

                parts.clear();
                break;
            }

            case 04: // delete_client;
            {
                Sockets.removeAll(parts[2]);
                MainWindow::Socket_delete(parts[2]);
                break;
            }

            case 05: // message
            {

                Add_New_Mesage(parts[3]);


                break;
            }

        }
    }
}

void MainWindow::SendToServer(const QString &str) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << str;
        socket->write(data);
        qDebug() << socket;
    } else {
        ui->textBrowser->append("Socket not connected");
    }
}

void MainWindow::on_pushButton_2_clicked() {
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_returnPressed() {
    QString message = QString("05,%1,%2,%3")
                          .arg(Interlocutor)
                          .arg(MySocket)
                          .arg(ui->lineEdit->text());

    SendToServer(message);

    ui->lineEdit->clear();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    QString num = item->text();
    Interlocutor = num;
}

void MainWindow::Socket_print() {
    for (auto i : Sockets) {
        QList<QListWidgetItem *> item =
            ui->listWidget->findItems(i, Qt::MatchExactly);
        if (item.isEmpty()) {
            ui->listWidget->addItem(i);
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

QString MainWindow::Load_HTML_from_File(){
    QFile file("text_brouser.html"); // Путь к файлу, убедитесь, что он правильный
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Ошибка открытия файла:" << file.errorString();
    } else {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();


        return content;
    }
}

QString MainWindow::LoadBaseTemplate() {
    return R"(
<html>
<head>
    <style>
        html, body {
            height: 100%; /* Устанавливаем высоту 100% для всех родительских элементов */
            margin: 0; /* Убираем отступы по умолчанию */
            padding: 0; /* Убираем внутренние отступы */
            font-family: Arial, sans-serif;
        }

        .container {
            width: 100%;
            padding: 5px;
            margin: 5px 0;
        }

        .right {
            text-align: right;
            background-color: lightblue;
            border-radius: 10px;
            padding: 10px;
            margin-right: 50px;
        }

        .left {
            text-align: left;
            background-color: lightgreen;
            border-radius: 10px;
            padding: 10px;
            margin-left: 50px;
        }
    </style>
</head>
<body>
    <!-- Место для сообщений -->
</body>
</html>
)";
}


QString MainWindow::Add_New_Mesage(QString mesage){
    // Загружаем текущий HTML контент (общая разметка)
    QString htmlContent = ui->textBrowser->toHtml();

    // Если это первое сообщение, то загружаем базовый шаблон
    if (htmlContent.isEmpty()) {
        htmlContent = LoadBaseTemplate();
    }

    // Создаем новый HTML блок для сообщения
    QString newHtmlBlock = QString(R"(
<div class="container">
    <div align="right" style="background-color: lightblue; border-radius: 10px; padding: 10px; margin-right: 50px;">%1</div>
</div>
)").arg(mesage);


    // Вставляем новое сообщение в конец
    int bodyIndex = htmlContent.indexOf("</body>");
    if (bodyIndex != -1) {
        htmlContent = htmlContent.left(bodyIndex) + newHtmlBlock + htmlContent.mid(bodyIndex);
        ui->textBrowser->setHtml(htmlContent);
    }
}
