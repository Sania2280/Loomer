#include "reconnection.h"
#include "regwindow.h"
#include "ui_RegWindow.h"
#include "config.h"

#include <QTimer>
#include <QDebug>
#include <QListWidget>

// Додайте оголошення зовнішньої змінної, якщо вона десь існує:


// Додайте правильний include або forward declaration для ServerConnector



Reconnection::Reconnection(QTcpSocket* socket, RegWindow* rWindow)
    : QObject(nullptr),
    m_socket(socket),
    m_regWind(rWindow) {
    connect(m_socket, &QTcpSocket::connected, this, &Reconnection::onConnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &Reconnection::onError);
    connect(m_socket, &QTcpSocket::disconnected, this, &Reconnection::onDisconnected);
}


void Reconnection::SetUpConnection(){

    m_socket->abort();
    m_socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);
}


void Reconnection::onConnected() {
    qDebug() << "Connected to Server";

    if (m_regWind) {
        m_regWind->ui->listWidget_errors->clear();
        m_regWind->ui->listWidget_errors->addItem("Connected to Server");
        QTimer::singleShot(3000, m_regWind->ui->listWidget_errors, &QListWidget::clear);
    }
}

void Reconnection::onError() {
    if (Close_Window_stat) return;

    qWarning() << "Error connect to Server:" << m_socket->errorString();
    QTimer::singleShot(3000, this, &Reconnection::SetUpConnection);
}

void Reconnection::onDisconnected() {
    if (Close_Window_stat) return;

    qWarning() << "Disconnected from Server. Reconnecting...";
    QTimer::singleShot(3000, this, [this]() {
        Reconnection::SetUpConnection();

        if (m_regWind) {
            m_regWind->ui->listWidget_errors->clear();
            m_regWind->ui->listWidget_errors->addItem("ERROR: lost connection");
        }
    });
}
