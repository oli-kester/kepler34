#include "SongSequenceGrid.hpp"

SongSequenceGrid::SongSequenceGrid(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);

    //start refresh timer to queue regular redraws
    mTimer = new QTimer(this);
    mTimer->setInterval(1);
    QObject::connect(mTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    mTimer->start();

}

void SongSequenceGrid::paintEvent(QPaintEvent *)
{
    mPainter = new QPainter(this);
    mBrush = new QBrush(Qt::NoBrush);
    mPen = new QPen(Qt::black);
    mFont.setPointSize(6);
    mPainter->setPen(*mPen);
    mPainter->setBrush(*mBrush);
    mPainter->setFont(mFont);

}

int SongSequenceGrid::snap() const
{
    return m_snap;
}

void SongSequenceGrid::setSnap(int snap)
{
    m_snap = snap;
}

QSize SongSequenceGrid::sizeHint() const
{
    return QSize(5000, 5000);
}

void SongSequenceGrid::mousePressEvent(QMouseEvent *event)
{

}

void SongSequenceGrid::mouseReleaseEvent(QMouseEvent *event)
{

}

void SongSequenceGrid::mouseMoveEvent(QMouseEvent *event)
{

}

void SongSequenceGrid::keyPressEvent(QKeyEvent *event)
{

}

void SongSequenceGrid::keyReleaseEvent(QKeyEvent *event)
{

}
