#include "PreferencesDialog.hpp"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(MidiPerformance *perf,
                                     QWidget *parent):
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    mPerf(perf)
{
    ui->setupUi(this);

    connect(ui->btnJackConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(jackConnect()));

    connect(ui->btnJackDisconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(jackDisconnect()));

    connect(ui->chkJackTransport,
            SIGNAL(clicked(bool)),
            this,
            SLOT(updateTransportSupport(bool)));

    connect(ui->chkJackConditional,
            SIGNAL(clicked(bool)),
            this,
            SLOT(updateMasterCond(bool)));

    connect(ui->chkJackMaster,
            SIGNAL(clicked(bool)),
            this,
            SLOT(updateTimeMaster(bool)));
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
    for (int c = 0; c<10; c++)
    {
        if (recent_files[c] == path)
        {
            path_found_index = c;
        }
    }

    //shift the recent files along by one, thus dropping the oldest
    for (int c = path_found_index; c>0; c--)
    {
        recent_files[c] = recent_files[c-1];
    }

    //add the new path to the first slot
    recent_files[0] = path;
}

void PreferencesDialog::jackConnect()
{
    mPerf->init_jack();
}

void PreferencesDialog::jackDisconnect()
{
    mPerf->deinit_jack();
}

void PreferencesDialog::updateMasterCond(bool newState)
{
    global_with_jack_master_cond = newState;
}

void PreferencesDialog::updateTimeMaster(bool newState)
{
    global_with_jack_master = newState;
}

void PreferencesDialog::updateTransportSupport(bool newState)
{
    global_with_jack_transport = newState;
}

