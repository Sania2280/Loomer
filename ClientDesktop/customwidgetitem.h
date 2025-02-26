#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "enums.h"

class CustomListItem : public QWidget
{
    Q_OBJECT
public:
    explicit CustomListItem(const QString &text, QWidget *parent = nullptr);

signals:
    void specialSymbolClicked(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;  // Добавьте это объявление
private:
    QLabel *label;
    QLabel *specialSymbolLabel;
    QString Style_Sheete();

    static QString Style ;
};

#endif // CUSTOMLISTWIDGET_H
