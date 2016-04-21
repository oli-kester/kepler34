#include "SongFrame.hpp"
#include "ui_SongFrame.h"

SongFrame::SongFrame(MidiPerformance *a_perf,
                     QWidget *parent):
    QFrame(parent),
    ui(new Ui::SongFrame),
    m_mainperf(a_perf)
{
    ui->setupUi(this);

    setSizePolicy(QSizePolicy::Expanding,
                  QSizePolicy::Expanding);

    // fill options for grid snap combo box and set default
    for (int i = 0; i < 6; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->cmbGridSnap->insertItem(i, combo_text);
    }
    ui->cmbGridSnap->setCurrentIndex(3);

    m_scroll_area = new QScrollArea(this);
    ui->vbox_centre->addWidget(m_scroll_area);

    mContainer = new QWidget(m_scroll_area);
    m_layout_grid = new QGridLayout(mContainer);
    mContainer->setLayout(m_layout_grid);

    m_palette = new QPalette();
    m_palette->setColor(QPalette::Background, Qt::darkGray);
    mContainer->setPalette(*m_palette);

    m_perfnames = new SongSequenceNames(m_mainperf, mContainer);
    m_perfroll = new SongSequenceGrid(m_mainperf, mContainer);
    m_perftime = new SongTimeBar(m_mainperf, mContainer);

    m_layout_grid->setSpacing(0);
    m_layout_grid->addWidget(m_perfnames, 1, 0, 1, 1);
    m_layout_grid->addWidget(m_perftime, 0, 1, 1, 1);
    m_layout_grid->addWidget(m_perfroll, 1, 1, 1, 1);
    m_layout_grid->setAlignment(m_perfroll, Qt::AlignTop);

    m_scroll_area->setWidget(mContainer);

    connect(ui->cmbGridSnap,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateGridSnap(int)));

    connect(ui->btnUndo,
            SIGNAL(clicked(bool)),
            m_perfroll,
            SLOT(undo()));

    connect(ui->btnRedo,
            SIGNAL(clicked(bool)),
            m_perfroll,
            SLOT(redo()));

    connect(ui->cmbGridSnap,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateGridSnap(int)));

    connect(ui->btnZoomIn,
            SIGNAL(clicked(bool)),
            this,
            SLOT(zoomIn()));

    connect(ui->btnZoomOut,
            SIGNAL(clicked(bool)),
            this,
            SLOT(zoomOut()));

    connect(ui->btnCollapse,
            SIGNAL(clicked(bool)),
            this,
            SLOT(markerCollapse()));

    connect(ui->btnExpand,
            SIGNAL(clicked(bool)),
            this,
            SLOT(markerExpand()));

    connect(ui->btnExpandCopy,
            SIGNAL(clicked(bool)),
            this,
            SLOT(markerExpandCopy()));

    connect(ui->btnLoop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(markerLoop(bool)));

    m_snap = 8;
    mBeatsPerMeasure = 4;
    mBeatWidth = 4;

    setSnap( 8 );
    setBeatsPerMeasure( 4 );
    setBeatLength( 4 );
}

SongFrame::~SongFrame()
{
    delete ui;
}

int SongFrame::getBeatWidth() const
{
    return mBeatWidth;
}

void SongFrame::updateGridSnap(int snapIndex)
{
    int snap;
    switch (snapIndex)
    {
    case 0:
        snap = 1;
        break;
    case 1:
        snap = 2;
        break;
    case 2:
        snap = 4;
        break;
    case 3:
        snap = 8;
        break;
    case 4:
        snap = 16;
        break;
    case 5:
        snap = 32;
        break;
    }

    m_snap = snap;
    setGuides();
}

void SongFrame::setSnap( int a_snap  )
{
    char b[10];
    snprintf( b, sizeof(b), "1/%d", a_snap );
    ui->cmbGridSnap->setCurrentText(b);

    m_snap = a_snap;
    setGuides();
}

void SongFrame::setBeatsPerMeasure( int a_beats_per_measure )
{
    //    char b[10];
    //    snprintf(b, sizeof(b), "%d", a_beats_per_measure );
    //    m_entry_bpm->set_text(b);

    mBeatsPerMeasure = a_beats_per_measure;
    setGuides();
}

int SongFrame::getBeatsPerMeasure() const
{
    return mBeatsPerMeasure;
}


void SongFrame::setBeatLength( int a_beat_width )
{
    //    char b[10];
    //    snprintf(b, sizeof(b), "%d", a_beat_width );
    //    m_entry_bw->set_text(b);

    mBeatWidth = a_beat_width;
    setGuides();
}

void SongFrame::setGuides()
{
    long measure_ticks = (c_ppqn * 4) * mBeatsPerMeasure / mBeatWidth;
    long snap_ticks =  measure_ticks / m_snap;
    long beat_ticks = (c_ppqn * 4) / mBeatWidth;
    m_perfroll->set_guides(snap_ticks, measure_ticks, beat_ticks);
    m_perftime->set_guides(snap_ticks, measure_ticks);
}

void SongFrame::zoomIn()
{
    m_perftime->zoomIn();
    m_perfroll->zoomIn();
}

void SongFrame::zoomOut()
{
    m_perftime->zoomOut();
    m_perfroll->zoomOut();
}

void SongFrame::updateSizes()
{
    m_perfroll->updateGeometry();
    m_perftime->updateGeometry();
    mContainer->adjustSize();
}

void SongFrame::markerCollapse()
{
    m_mainperf->push_trigger_undo();
    m_mainperf->move_triggers( false );
}

void SongFrame::markerExpand()
{
    m_mainperf->push_trigger_undo();
    m_mainperf->move_triggers( true );
}

void SongFrame::markerExpandCopy()
{
    m_mainperf->push_trigger_undo();
    m_mainperf->copy_triggers();
}

void SongFrame::markerLoop(bool loop)
{
    m_mainperf->set_looping(loop);
}
