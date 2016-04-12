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
    explicit PreferencesDialog(MidiPerformance *perf,
                               QWidget *parent = 0);
    ~PreferencesDialog();

    //add a new file to the recent files list.
    //reorganises the list if file already present
    void addRecentFile(QString path);

private:
    Ui::PreferencesDialog *ui;

    MidiPerformance *mPerf;

private slots:
    void updateTransportSupport(bool newState);
    void updateTimeMaster(bool newState);
    void updateMasterCond(bool newState);
    void jackConnect();
    void jackDisconnect();
};

#endif // PREFERENCESDIALOG_HPP
