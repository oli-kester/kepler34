#include "SongFrame.hpp"
#include "ui_SongFrame.h"

SongFrame::SongFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SongFrame)
{
    ui->setupUi(this);

    // fill options for grid snap combo box and set default
    for (int i = 0; i < 5; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->combo_grid_snap->insertItem(i, combo_text);
    }
    ui->combo_grid_snap->setCurrentIndex(3);
}

SongFrame::~SongFrame()
{
    delete ui;
}
