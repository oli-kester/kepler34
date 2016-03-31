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

    m_msg_error = new QErrorMessage(this);

    m_msg_save_changes = new QMessageBox(this);
    m_msg_save_changes->setText(tr("Unsaved changes detected."));
    m_msg_save_changes->setInformativeText(tr("Do you want to save them?"));
    m_msg_save_changes->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    m_msg_save_changes->setDefaultButton(QMessageBox::Save);

    m_dialog_prefs = new PreferencesDialog(this);
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
                     SLOT(refresh()));
    m_timer->start();

    //connect GUI elements to handlers
    QObject::connect(ui->actionNew,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(newFile()));

    QObject::connect(ui->actionSave,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(saveFile()));

    QObject::connect(ui->actionSave_As,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(saveFileAs()));

    QObject::connect(ui->actionImport_MIDI,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(showImportDialog()));

    QObject::connect(ui->actionOpen,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(showOpenFileDialog()));

    QObject::connect(ui->actionAbout,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(showAboutDialog()));

    QObject::connect(ui->actionAbout_Qt,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(showAboutQtDialog()));

    QObject::connect(ui->actionPreferences,
                     SIGNAL(triggered(bool)),
                     m_dialog_prefs,
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

    QObject::connect(ui->spinBpm,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(updateBpm(int)));


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

void MainWindow::updateBpm(int newBpm)
{
    m_main_perf->set_bpm(newBpm);
    m_modified = true;
}

void MainWindow::showOpenFileDialog()
{
    //TODO check if file is saved before opening anew
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
        QString msg_text = "Error reading MIDI data from file: " + path;
        m_msg_error->showMessage(msg_text);
        m_msg_error->exec();
        return;
    }

    //set last used dir to the one we have just loaded from
    int last_slash = path.lastIndexOf("/");
    last_used_dir = path.left(last_slash + 1);
    qDebug() << "New recent directory"
             << last_used_dir << endl;
    global_filename = path;

    updateWindowTitle();

    //add to recent files list
    m_dialog_prefs->addRecentFile(path);

    //update recent menu
    //    redraw_menu();

    m_live_frame->redraw();
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

void MainWindow::refresh()
{
    m_beat_ind->update();
}

bool MainWindow::saveCheck()
{
    bool result = false;

    if (m_modified) {
        int choice = m_msg_save_changes->exec();
        switch (choice) {
        case QMessageBox::Save:
            if (saveFile())
                result = true;
            break;
        case QMessageBox::Discard:
            result = true;
            break;
        case QMessageBox::Cancel:
        default:
            break;
        }
    }
    else
        result = true;

    return result;
}

void MainWindow::newFile()
{
    if (saveCheck())
    {
        m_main_perf->clear_all();

        //TODO reinstate these
        //        m_main_wid->reset();
        //        m_entry_notes->set_text( * m_mainperf->get_screen_set_notepad(
        //                                     m_mainperf->get_screenset() ));

        global_filename = "";
        updateWindowTitle();
        m_modified = false;
    }
}

bool MainWindow::saveFile()
{
    bool result = false;

    if (global_filename == "") {
        saveFileAs();
        return true;
    }

    MidiFile file(global_filename);
    result = file.write(m_main_perf);

    if (!result) {
        m_msg_error->showMessage("Error writing file.");
        m_msg_error->exec();
    } else {
        /* add to recent files list */
        m_dialog_prefs->addRecentFile(global_filename);
        /* update recent menu */
        //TODO reinstate this
        //        redraw_menu();
    }
    m_modified = !result;
    return result;

}

void MainWindow::saveFileAs()
{
    QString file;

    file = QFileDialog::getSaveFileName(
                this,
                tr("Save MIDI file as..."),
                last_used_dir,
                tr("MIDI files (*.midi *.mid);;"
                   "All files (*)")
                );

    if (!file.isEmpty())
    {
        QFileInfo fileInfo(file);
        QString suffix = fileInfo.completeSuffix();

        if ((suffix != "midi") && (suffix != "mid"))
        {
            file += ".midi";
        }

        global_filename = file;
        updateWindowTitle();
        saveFile();
    }
}

//TODO
void MainWindow::showImportDialog()
{

}

void MainWindow::showAboutDialog()
{

}

void MainWindow::showAboutQtDialog()
{

}
