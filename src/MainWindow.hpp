#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <iostream>

#include "LiveFrame.hpp"
#include "SongFrame.hpp"
#include "EditFrame.hpp"
#include "PreferencesDialog.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow      *ui;

    LiveFrame           *live_frame;
    SongFrame           *song_frame;
    EditFrame           *edit_frame;

    PreferencesDialog   *prefs_dialog;
};

#endif // MAINWINDOW_HPP
