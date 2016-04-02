#include "EditTimeBar.hpp"

EditTimeBar::EditTimeBar(MidiSequence *a_seq, QWidget *parent):
    QWidget(parent),
    m_seq(a_seq)
{
    //start refresh timer to queue regular redraws
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    m_timer->start();
}

void EditTimeBar::paintEvent(QPaintEvent *event)
{

}

void EditTimeBar::mousePressEvent(QMouseEvent *event)
{

}

void EditTimeBar::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditTimeBar::mouseMoveEvent(QMouseEvent *event)
{

}
