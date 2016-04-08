#include "SongFrame.hpp"
#include "ui_SongFrame.h"

SongFrame::SongFrame(MidiPerformance *a_perf,
                     QWidget *parent):
    QFrame(parent),
    m_mainperf(a_perf),
    ui(new Ui::SongFrame)
{
    ui->setupUi(this);

    setSizePolicy(QSizePolicy::Expanding,
                  QSizePolicy::Expanding);

    // fill options for grid snap combo box and set default
    for (int i = 0; i < 5; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->combo_grid_snap->insertItem(i, combo_text);
    }
    ui->combo_grid_snap->setCurrentIndex(3);

    m_scroll_area = new QScrollArea(this);
    ui->vbox_centre->addWidget(m_scroll_area);

    mContainer = new QWidget(m_scroll_area);
    m_layout_grid = new QGridLayout(mContainer);
    mContainer->setLayout(m_layout_grid);

    m_palette = new QPalette();
    m_palette->setColor(QPalette::Background, Qt::darkGray);
    mContainer->setPalette(*m_palette);

    m_perfnames = new SongSequenceNames(mContainer);
    m_perfroll = new SongSequenceGrid(mContainer);
    m_perftime = new SongTimeBar(mContainer);

    m_layout_grid->setSpacing(0);
    m_layout_grid->addWidget(m_perfnames, 1, 0, 1, 1);
    m_layout_grid->addWidget(m_perftime, 0, 1, 1, 1);
    m_layout_grid->addWidget(m_perfroll, 1, 1, 1, 1);
    m_layout_grid->setAlignment(m_perfroll, Qt::AlignTop);

    m_scroll_area->setWidget(mContainer);

    connect(ui->combo_grid_snap,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateGridSnap(int)));
}

SongFrame::~SongFrame()
{
    delete ui;
}

void SongFrame::updateGridSnap(int snap)
{

}
