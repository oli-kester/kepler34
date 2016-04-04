#ifndef CONTAINERWIDGET_HPP
#define CONTAINERWIDGET_HPP

#include <QWidget>

class ContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContainerWidget(QWidget *parent = 0);

protected:
    //override the sizehint to set our own defaults
    QSize sizeHint() const;

signals:

public slots:
};

#endif // CONTAINERWIDGET_HPP
