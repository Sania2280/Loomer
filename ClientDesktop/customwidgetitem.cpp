#include "customwidgetitem.h"
#include "getpath.h"

#include <QPixmap>
#include <QStyleOption>
#include <QPainter>
#include <QFile>

QString CustomListItem::Style = "";

CustomListItem::CustomListItem(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    label = new QLabel(text, this);

    Get_My_Path get_my_path;


    label->setStyleSheet(Style_Sheete());
    label->adjustSize();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(label);

    int widgetWidth = label->width() + 20;
    setFixedWidth(widgetWidth);

}

void CustomListItem::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

}

QString CustomListItem::Style_Sheete() {
    if (Style.isEmpty() ){
        QFile styleFile("./styles/item_style.qss"); // Убедитесь, что путь корректный
        if (styleFile.open(QFile::ReadOnly)) {
            Style = QLatin1String(styleFile.readAll());
            styleFile.close();
        }
        else qDebug() << "Style File not open";
    }
    else qDebug() << "Style Sheet is wrinen";

    return Style;

}
