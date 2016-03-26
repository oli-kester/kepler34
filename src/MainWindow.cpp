#include "MainWindow.hpp"
#include "ui_MainWindow.h"

bool is_pattern_playing = false;

MainWindow::MainWindow(QWidget *parent, MidiPerformance *a_p ) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_main_perf(a_p)
{
    m_ui->setupUi(this);

    m_modified = false;

    m_prefs_dialog = new PreferencesDialog(this);

    m_live_frame = new LiveFrame(m_ui->LiveTab, m_main_perf);
    m_song_frame = new SongFrame(m_ui->SongTab);
    m_edit_frame = new EditFrame(m_ui->EditTab);

    m_ui->LiveTabLayout->addWidget(m_live_frame);
    m_ui->SongTabLayout->addWidget(m_song_frame);
    m_ui->EditTabLayout->addWidget(m_edit_frame);

    //connect GUI elements to handlers
    QObject::connect(m_ui->actionOpen,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(showOpenFileDialog()));
    QObject::connect(m_ui->actionPreferences,
                     SIGNAL(triggered(bool)),
                     m_prefs_dialog,
                     SLOT(show()));

    QObject::connect(m_ui->btnPlay,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(startPlaying()));
    QObject::connect(m_ui->btnStop,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(stopPlaying()));
    QObject::connect(m_ui->btnRecord,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(setRecording(bool)));

    show();
}

MainWindow::~MainWindow()
{
    delete m_ui;
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

    m_ui->btnPlay->setChecked(false);

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
    //    dialog.setFileMode(QFileDialog::ExistingFile);
    //    dialog.setNameFilter(tr(
    //                             "MIDI files (*.midi *.mid);;"
    //                             "All files (*)"));
    //    dialog.setDirectory(last_used_dir);
    //    dialog.setViewMode(QFileDialog::Detail);

    //    if (dialog.exec())
    //        files=dialog.selectedFiles();

    file = QFileDialog::getOpenFileName(
                this,
                tr("Open MIDI file"),
                last_used_dir,
                tr("MIDI files (*.midi *.mid);;"
                   "All files (*)")
                );


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
    }

}
