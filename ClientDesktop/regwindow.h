#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QDialog>
#include <QTcpSocket>


class MainWindow;

class ServerConnector;

namespace Ui {
class regwindow;
}

class RegWindow : public QDialog
{
    Q_OBJECT

public:

    Ui::regwindow *ui;
    QString Style_Sheete();

    explicit RegWindow(QWidget *parent = nullptr);
    ~RegWindow();

private slots:
    void on_pushButton_Log_in_clicked();
    void on_pushButton_Sing_up_clicked();
    void on_pushButton_return_clicked();

public slots:
    void on_pushButton_end_clicked();



private:
    ServerConnector *serverConnector;

    void ShowMainForm();
    void StartUpWindow();

    bool Close_Window_stat = false;

    QTcpSocket* socket;
signals:
    void CloseWindow();

protected:
    using QDialog::closeEvent;
    void closeEvent(QCloseEvent *event) override;

};

#endif // REGWINDOW_H
