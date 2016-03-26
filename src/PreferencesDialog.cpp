#include "PreferencesDialog.hpp"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::addRecentFile(QString path)
{
    //start shifting from the last element if file not already recent
    int path_found_index = 9;

    //check if path is already present
    for (int c = 0; c<10; c++) {
        if (recent_files[c] == path) {
            path_found_index = c;
        }
    }

    //shift the recent files along by one, thus dropping the oldest
    for (int c = path_found_index; c>0; c--){
        recent_files[c] = recent_files[c-1];
    }

    //add the new path to the first slot
    recent_files[0] = path;
}
