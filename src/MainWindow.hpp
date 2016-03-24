#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDebug>

#include "LiveFrame.hpp"
#include "SongFrame.hpp"
#include "EditFrame.hpp"
#include "PreferencesDialog.hpp"
#include "MidiPerformance.hpp"

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
    Ui::MainWindow      *ui;

    LiveFrame           *live_frame;
    SongFrame           *song_frame;
    EditFrame           *edit_frame;

    PreferencesDialog   *prefs_dialog;

    MidiPerformance     *main_perf;

private slots:
    void startPlaying();
    void stopPlaying();
    void setRecording(bool record);
};

#endif // MAINWINDOW_HPP
