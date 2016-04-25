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
    //makes sure the dialog properly reflects internal settings
    void syncWithInternals();

    //backup preferences incase we cancel changes
    void backup();

    Ui::PreferencesDialog *ui;

    MidiPerformance *mPerf;

    //backup variables for settings
    bool backupJackTransport;
    bool backupTimeMaster;
    bool backupMasterCond;
    bool backupNoteResume;
    int  backupKeyHeight;

private slots:
    void updateTransportSupport();
    void updateTimeMaster();
    void updateMasterCond();
    void jackConnect();
    void jackDisconnect();
    void okay();
    void cancel();
    void updateNoteResume();
    void updateKeyHeight();
};

#endif // PREFERENCESDIALOG_HPP
