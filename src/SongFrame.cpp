#include "SongFrame.hpp"
#include "ui_SongFrame.h"

SongFrame::SongFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SongFrame)
{
    ui->setupUi(this);
}

SongFrame::~SongFrame()
{
    delete ui;
}
