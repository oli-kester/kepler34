#include "ContainerWidget.hpp"

ContainerWidget::ContainerWidget(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::MinimumExpanding);
}

QSize ContainerWidget::sizeHint() const
{
    return QSize(5000,5000);
}
