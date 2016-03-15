#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    live_frame = new LiveFrame(ui->LiveTab);
    song_frame = new SongFrame(ui->SongTab);
    edit_frame = new EditFrame(ui->EditTab);

    ui->LiveTabLayout->addWidget(live_frame);
    ui->SongTabLayout->addWidget(song_frame);
    ui->EditTabLayout->addWidget(edit_frame);

    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
