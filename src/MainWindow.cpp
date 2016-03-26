#include "MainWindow.hpp"
#include "ui_MainWindow.h"

bool is_pattern_playing = false;

MainWindow::MainWindow(QWidget *parent, MidiPerformance *a_p ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    main_perf(a_p)
{
    ui->setupUi(this);


    prefs_dialog = new PreferencesDialog(this);

    live_frame = new LiveFrame(ui->LiveTab, main_perf);
    song_frame = new SongFrame(ui->SongTab);
    edit_frame = new EditFrame(ui->EditTab);

    ui->LiveTabLayout->addWidget(live_frame);
    ui->SongTabLayout->addWidget(song_frame);
    ui->EditTabLayout->addWidget(edit_frame);

    //connect GUI elements to handlers
    QObject::connect(ui->actionPreferences,SIGNAL(triggered()),prefs_dialog,SLOT(show()));

    QObject::connect(ui->btnPlay, SIGNAL(clicked(bool)), this, SLOT(startPlaying()));
    QObject::connect(ui->btnStop, SIGNAL(clicked(bool)), this, SLOT(stopPlaying()));
    QObject::connect(ui->btnRecord, SIGNAL(clicked(bool)), this, SLOT(setRecording(bool)));

    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startPlaying()
{
    main_perf->set_playback_mode(false);
    main_perf->position_jack(false);
    main_perf->start(false);
    main_perf->start_jack();
    is_pattern_playing = true;

    qDebug() << "Start playing (Live)" << endl;
}

void MainWindow::stopPlaying()
{
    main_perf->stop_jack();
    main_perf->stop();

    ui->btnPlay->setChecked(false);

    qDebug() << "Stop playing" << endl;
}

void MainWindow::setRecording(bool record)
{
    main_perf->set_song_recording(record);

    qDebug() << "Set recording - " << record << endl;
}
