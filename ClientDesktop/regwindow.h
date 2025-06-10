#ifndef REGWINDOW_H
#define REGWINDOW_H

#include "enums.h"
#include <QDialog>
#include <QTcpSocket>




class MainWindow;
class DataSender;

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
    void logSingWorker(MesageIdentifiers id);
    void connectStatWorker(ConnectionStat stat);


private:
    DataSender *dataSendler;

    void ShowMainForm();
    void StartUpWindow();

    bool Close_Window_stat = false;
    bool LogInWindStat = false;
    bool SingUpWindStat = false;

    QTcpSocket* socket;
signals:
    void CloseWindow();

protected:
    using QDialog::closeEvent;
    void closeEvent(QCloseEvent *event)  override;
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // REGWINDOW_H
