#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>

#include "LiveFrame.hpp"
#include "SongFrame.hpp"
#include "EditFrame.hpp"
#include "PreferencesDialog.hpp"
#include "MidiPerformance.hpp"
#include "MidiFile.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, MidiPerformance *a_p = 0);
    ~MainWindow();

    void openMidiFile(const QString& path);

private:
    Ui::MainWindow      *m_ui;

    LiveFrame           *m_live_frame;
    SongFrame           *m_song_frame;
    EditFrame           *m_edit_frame;

    PreferencesDialog   *m_prefs_dialog;

    MidiPerformance     *m_main_perf;

    bool                m_modified;

private slots:
    void startPlaying();
    void stopPlaying();
    void setRecording(bool record);
    void showOpenFileDialog();

};

#endif // MAINWINDOW_HPP
