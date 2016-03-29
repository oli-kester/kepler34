#include "MainWindow.hpp"
#include "ui_MainWindow.h"

bool is_pattern_playing = false;

MainWindow::MainWindow(QWidget *parent, MidiPerformance *a_p ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_main_perf(a_p)
{
    ui->setupUi(this);

    m_modified = false;

    m_error_dialog = new QErrorMessage(this);

    m_prefs_dialog = new PreferencesDialog(this);

    m_live_frame = new LiveFrame(ui->LiveTab, m_main_perf);
    m_song_frame = new SongFrame(ui->SongTab);
    m_edit_frame = new EditFrame(ui->EditTab);

    m_beat_ind = new BeatIndicator(this, m_main_perf, 4, 4);
    
    ui->layoutTransport->addWidget(m_beat_ind);

    ui->LiveTabLayout->addWidget(m_live_frame);
    ui->SongTabLayout->addWidget(m_song_frame);
    ui->EditTabLayout->addWidget(m_edit_frame);

    //timer to refresh GUI elements every few ms
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(redraw()));
    m_timer->start();

    //connect GUI elements to handlers
    QObject::connect(ui->actionOpen,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(showOpenFileDialog()));

    QObject::connect(ui->actionPreferences,
                     SIGNAL(triggered(bool)),
                     m_prefs_dialog,
                     SLOT(show()));


    QObject::connect(ui->btnPlay,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(startPlaying()));

    QObject::connect(ui->btnStop,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(stopPlaying()));

    QObject::connect(ui->btnRecord,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(setRecording(bool)));

    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startPlaying()
{
    m_main_perf->set_playback_mode(false);
    m_main_perf->position_jack(false);
    m_main_perf->start(false);
    m_main_perf->start_jack();
    is_pattern_playing = true;

    qDebug() << "Start playing (Live)" << endl;
}

void MainWindow::stopPlaying()
{
    m_main_perf->stop_jack();
    m_main_perf->stop();

    ui->btnPlay->setChecked(false);

    qDebug() << "Stop playing" << endl;
}

void MainWindow::setRecording(bool record)
{
    m_main_perf->set_song_recording(record);

    qDebug() << "Set recording - " << record << endl;
}

void MainWindow::showOpenFileDialog()
{
    //TODO check if file is saved before opening new

    QFileDialog dialog(this);
    QString file;

    file = QFileDialog::getOpenFileName(
                this,
                tr("Open MIDI file"),
                last_used_dir,
                tr("MIDI files (*.midi *.mid);;"
                   "All files (*)")
                );

    //don't bother trying to open if the user cancels
    if (!file.isEmpty())
        openMidiFile(file);
}

void MainWindow::openMidiFile(const QString &path)
{
    bool result;

    m_main_perf->clear_all();

    MidiFile f(path);
    result = f.parse(m_main_perf, 0);
    m_modified = !result;

    if (!result) {
        //TODO error
        QString m_error_msg = "Error reading MIDI data from file: " + path;
        m_error_dialog->showMessage(m_error_msg);
        m_error_dialog->exec();
        return;
    }

    //set last used dir to the one we have just loaded from
    int last_slash = path.lastIndexOf("/");
    last_used_dir = path.left(last_slash + 1);
    qDebug() << "New recent directory"
             << last_used_dir << endl;
    global_filename = path;

    updateWindowTitle();

    //    add to recent files list
    m_prefs_dialog->addRecentFile(path);

    //update recent menu
    //    redraw_menu();

    //    m_main_wid->reset();
    //    m_entry_notes->set_text(*m_mainperf->get_screen_set_notepad(
    //                m_mainperf->get_screenset()));
    ui->spinBpm->setValue(m_main_perf->get_bpm());
}

void MainWindow::updateWindowTitle()
{
    QString title;

    if (global_filename == "")
        title = ( PACKAGE ) + QString( " - [unnamed]" );
    else
    {
        //give us a title with just the MIDI filename, after the last slash
        int last_slash = global_filename.lastIndexOf("/");
        title = global_filename.right(
                    global_filename.length() - last_slash - 1);
        qDebug() << "New window title - " << title << endl;
        title = ( PACKAGE )
                + QString( " - [" )
                + title
                + QString( "]" );
    }

    this->setWindowTitle(title);

}

void MainWindow::redraw()
{
    m_beat_ind->update();
}
