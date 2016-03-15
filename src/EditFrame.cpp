#include "EditFrame.hpp"
#include "ui_EditFrame.h"

EditFrame::EditFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EditFrame)
{
    ui->setupUi(this);
}

EditFrame::~EditFrame()
{
    delete ui;
}
