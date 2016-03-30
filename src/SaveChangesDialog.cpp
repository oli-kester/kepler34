#include "SaveChangesDialog.hpp"
#include "ui_SaveChangesDialog.h"

SaveChangesDialog::SaveChangesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveChangesDialog)
{
    ui->setupUi(this);
}

SaveChangesDialog::~SaveChangesDialog()
{
    delete ui;
}
