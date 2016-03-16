#include "EditFrame.hpp"
#include "ui_EditFrame.h"

EditFrame::EditFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EditFrame)
{
    ui->setupUi(this);

    // fill options for grid snap combo box and set default
    for (int i = 0; i < 5; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->cmb_grid_snap->insertItem(i, combo_text);
    }
    ui->cmb_grid_snap->setCurrentIndex(3);

    // fill options for beats per measure combo box and set default
    for (int i = 0; i < 16; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmb_beat_measure->insertItem(i, combo_text);
    }
    ui->cmb_beat_measure->setCurrentIndex(3);

    // fill options for beat length combo box and set default
    for (int i = 0; i < 5; i++)
    {
        QString combo_text = QString::number(pow(2,i));
        ui->cmb_beat_length->insertItem(i, combo_text);
    }
    ui->cmb_beat_length->setCurrentIndex(2);

    // fill options for MIDI channel numbers
    for (int i = 0; i <= 15; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmb_midi_chan->insertItem(i,combo_text);
    }
    ui->cmb_midi_chan->setCurrentIndex(0);

    // fill options for seq length
    for (int i = 0; i <= 15; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmb_midi_chan->insertItem(i,combo_text);
    }
    ui->cmb_midi_chan->setCurrentIndex(0);

}

EditFrame::~EditFrame()
{
    delete ui;
}
