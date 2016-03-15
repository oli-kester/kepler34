#include "LiveFrame.hpp"
#include "ui_LiveFrame.h"

LiveFrame::LiveFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LiveFrame)
{
    ui->setupUi(this);
}

LiveFrame::~LiveFrame()
{
    delete ui;
}
