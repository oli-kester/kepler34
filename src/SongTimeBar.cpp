#include "SongTimeBar.hpp"

SongTimeBar::SongTimeBar(QWidget *parent) : QWidget(parent)
{
    //start refresh timer to queue regular redraws
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    m_timer->start();

    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);
}

void SongTimeBar::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::black);
    m_brush = new QBrush(Qt::lightGray, Qt::SolidPattern),
    m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setBrush(*m_brush);
    m_painter->setFont(m_font);
}

QSize EditTimeBar::sizeHint() const
{
    return QSize(m_seq->getLength() + 100, 22);
}
