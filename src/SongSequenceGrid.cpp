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

