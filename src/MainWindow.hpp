#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QErrorMessage>
#include <QTimer>
#include <QMessageBox>
#include <QDesktopWidget>

#include "LiveFrame.hpp"
#include "SongFrame.hpp"
#include "EditFrame.hpp"
#include "PreferencesDialog.hpp"
#include "MidiPerformance.hpp"
#include "MidiFile.hpp"
#include "BeatIndicator.hpp"
#include "KeplerStyle.hpp"

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

    //open the file at the given path
    void openMidiFile(const QString& path);

protected:
    //override keyboard events for interaction
    void keyPressEvent          (QKeyEvent * event);

private:

    //check if the file has been modified.
    //if modified, ask the user whether to save changes
    bool saveCheck();

    //update window title from the global filename
    void updateWindowTitle();

    //update the recent files menu
    void updateRecentFilesMenu();

    Ui::MainWindow      *ui;

    LiveFrame           *m_live_frame;
    SongFrame           *m_song_frame;
    EditFrame           *m_edit_frame;

    QErrorMessage       *m_msg_error;
    QMessageBox         *m_msg_save_changes;
    QTimer              *m_timer;
    QAction             *mRecentFileActions[10];
    QMenu               *mRecentMenu;
    QFileDialog         *mImportDialog;

    MidiPerformance     *m_main_perf;
    BeatIndicator       *m_beat_ind;
    PreferencesDialog   *m_dialog_prefs;

    //TODO fully move this into main performance
    bool                 m_modified;

private slots:
    void startPlaying();
    void stopPlaying();
    void setSongPlayback(bool playSongData);
    void setRecording(bool record);
    void setRecordingSnap(bool snap);
    void updateBpm(int newBpm);
    void updateBeatsPerMeasure(int bmIndex);
    void updateBeatLength(int blIndex);
    void newFile();
    bool saveFile();
    void saveFileAs();
    void quit();
    void showImportDialog(); //import MIDI data from current bank onwards
    void showOpenFileDialog();
    void showAboutDialog();
    void showAboutQtDialog();
    void tabWidgetClicked(int newIndex);
    void load_recent_1();
    void load_recent_2();
    void load_recent_3();
    void load_recent_4();
    void load_recent_5();
    void load_recent_6();
    void load_recent_7();
    void load_recent_8();
    void load_recent_9();
    void load_recent_10();

    //redraw certain GUI elements
    void refresh();

    //set the editor to a specific seq
    //and switch tab to it
    void loadEditor(int seqId);
};

#endif // MAINWINDOW_HPP
