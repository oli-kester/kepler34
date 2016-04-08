#include "SongSequenceNames.hpp"

SongSequenceNames::SongSequenceNames(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::MinimumExpanding);
}

void SongSequenceNames::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::black);
    m_brush = new QBrush(Qt::SolidPattern);
    m_brush->setColor(Qt::lightGray);
    m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setBrush(*m_brush);
    m_painter->setFont(m_font);

}


QSize EditKeys::sizeHint() const
{
    return QSize(100, 5000);
}
