#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QErrorMessage>
#include <QTimer>

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
    void openMidiFile(const QString& path);
    void updateWindowTitle();

    Ui::MainWindow      *ui;

    LiveFrame           *m_live_frame;
    SongFrame           *m_song_frame;
    EditFrame           *m_edit_frame;

    QErrorMessage       *m_error_dialog;
    QTimer              *m_timer;

    MidiPerformance     *m_main_perf;
    BeatIndicator       *m_beat_ind;
    PreferencesDialog   *m_prefs_dialog;

    bool                 m_modified;

private slots:
    void startPlaying();
    void stopPlaying();
    void setRecording(bool record);
    void updateBpm(int newBpm);
    void showOpenFileDialog();
    void redraw(); //redraw certain GUI elements
};

#endif // MAINWINDOW_HPP
