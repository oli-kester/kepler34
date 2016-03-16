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

    // fill options for beats per measure combo box and set default
    for (int i = 0; i < 16; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->combo_beat_measure->insertItem(i, combo_text);
    }
    ui->combo_beat_measure->setCurrentIndex(3);

    // fill options for beat length combo box and set default
    for (int i = 0; i < 5; i++)
    {
        QString combo_text = QString::number(pow(2,i));
        ui->combo_beat_length->insertItem(i, combo_text);
    }
    ui->combo_beat_length->setCurrentIndex(2);


}

SongFrame::~SongFrame()
{
    delete ui;
}
