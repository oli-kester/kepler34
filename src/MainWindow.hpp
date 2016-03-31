#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QErrorMessage>
#include <QTimer>
#include <QMessageBox>

#include "LiveFrame.hpp"
#include "SongFrame.hpp"
#include "EditFrame.hpp"
#include "PreferencesDialog.hpp"
#include "MidiPerformance.hpp"
#include "MidiFile.hpp"
#include "BeatIndicator.hpp"

//the main window of Kepler34

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0, MidiPerformance *a_p = 0);
    ~MainWindow();

private:

    //check if the file has been modified.
    //if modified, ask the user whether to save changes
    bool saveCheck();

    //open the file at the given path
    void openMidiFile(const QString& path);

    //update window title from the global filename
    void updateWindowTitle();


    Ui::MainWindow      *ui;

    LiveFrame           *m_live_frame;
    SongFrame           *m_song_frame;
    EditFrame           *m_edit_frame;

    QErrorMessage       *m_msg_error;
    QMessageBox         *m_msg_save_changes;
    QTimer              *m_timer;

    MidiPerformance     *m_main_perf;
    BeatIndicator       *m_beat_ind;
    PreferencesDialog   *m_dialog_prefs;

    //TODO fully move this into main performance
    bool                 m_modified;

private slots:

    //TODO recent files function
    void startPlaying();
    void stopPlaying();
    void setRecording(bool record);
    void updateBpm(int newBpm);
    void newFile();
    bool saveFile();
    void saveFileAs();
    void showImportDialog();
    void showOpenFileDialog();
    void showAboutDialog();
    void showAboutQtDialog();
    void refresh(); //redraw certain GUI elements
};

#endif // MAINWINDOW_HPP
