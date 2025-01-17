    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotReadyRead();
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    void SendToServer(const QString &str);
    void Socket_print();
    void Socket_delete(QString socket_to_delete);

    QString Load_HTML_from_File();
    QString Add_New_Mesage(QString mesage);
    QString LoadBaseTemplate();

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString MySocket;
    QString Interlocutor;
    QVector<QString> Sockets;
};
#endif // MAINWINDOW_H
