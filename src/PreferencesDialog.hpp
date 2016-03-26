#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

#include "MidiPerformance.hpp"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

    //add a new file to the recent files list.
    //reorganises the list if file already present
    void addRecentFile(QString path);

private:
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_HPP
