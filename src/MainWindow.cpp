#include "MainWindow.hpp"
#include "ui_MainWindow.h"

bool is_pattern_playing = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    prefs_dialog = new PreferencesDialog(this);

    live_frame = new LiveFrame(ui->LiveTab);
    song_frame = new SongFrame(ui->SongTab);
    edit_frame = new EditFrame(ui->EditTab);

    ui->LiveTabLayout->addWidget(live_frame);
    ui->SongTabLayout->addWidget(song_frame);
    ui->EditTabLayout->addWidget(edit_frame);

    QObject::connect(ui->actionPreferences,SIGNAL(triggered()),prefs_dialog,SLOT(show()));

    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
