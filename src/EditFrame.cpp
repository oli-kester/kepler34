#include "EditFrame.hpp"
#include "ui_EditFrame.h"

EditFrame::EditFrame(QWidget *parent, MidiPerformance *perf, MidiSequence *seq) :
    QFrame(parent),
    m_perf(perf),
    m_seq(seq),
    ui(new Ui::EditFrame)
{
    ui->setupUi(this);

    // fill options for grid snap combo box and set default
    for (int i = 0; i < 5; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->cmb_grid_snap->insertItem(i, combo_text);
    }
    ui->cmb_grid_snap->setCurrentIndex(3);

    // fill options for MIDI channel numbers
    for (int i = 0; i <= 15; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmb_midi_chan->insertItem(i,combo_text);
    }

    // fill options for seq length
    for (int i = 0; i <= 15; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmb_seq_len->insertItem(i,combo_text);
    }
    ui->cmb_seq_len->insertItem(16,"32");
    ui->cmb_seq_len->insertItem(17,"64");

    // fill options for scale
    ui->cmb_scale->insertItem(0,"Off");
    ui->cmb_scale->insertItem(1,"Major");
    ui->cmb_scale->insertItem(2,"Minor");

    // pull data from sequence object
    ui->txt_seq_name->setPlainText(m_seq->get_name());
    ui->cmb_midi_chan->setCurrentIndex(m_seq->get_midi_channel());

    m_seq->set_editing(true);

    m_scroll_area = new QScrollArea(this);
    m_scroll_area->setBackgroundRole(QPalette::Light);
    ui->vbox_centre->addWidget(m_scroll_area);

    m_container = new QWidget(m_scroll_area);
    m_layout_grid = new QGridLayout(m_container);
    m_container->setLayout(m_layout_grid);

    m_seqkeys_wid = new EditKeys(m_seq, m_container);
    m_seqtime_wid = new EditTimeBar(m_seq, m_container);
    m_seqroll_wid = new EditNoteRoll(m_perf, m_seq, m_container);

    m_layout_grid->setSpacing(0);
    m_layout_grid->addWidget(m_seqkeys_wid, 1, 0, 1, 1);
    m_layout_grid->addWidget(m_seqtime_wid, 0, 1, 1, 1);
    m_layout_grid->addWidget(m_seqroll_wid, 1, 1, 1, 1);
    m_layout_grid->setAlignment(m_seqroll_wid, Qt::AlignTop);

    m_scroll_area->setWidget(m_container);

    //connect all the UI signals
    connect(ui->txt_seq_name,
            SIGNAL(textChanged()),
            this,
            SLOT(updateSeqName()));

    connect(ui->cmb_grid_snap,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateGridSnap(int)));

    connect(ui->cmb_midi_bus,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateMidiBus(int)));

    connect(ui->cmb_midi_chan,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateMidiChannel(int)));

    connect(ui->btn_undo,
            SIGNAL(clicked(bool)),
            this,
            SLOT(undo()));

    connect(ui->btn_redo,
            SIGNAL(clicked(bool)),
            this,
            SLOT(redo()));

    connect(ui->btn_tools,
            SIGNAL(clicked(bool)),
            this,
            SLOT(showTools()));

    connect(ui->cmb_note_len,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateNoteLength(int)));

    connect(ui->btn_zoom_in,
            SIGNAL(clicked(bool)),
            this,
            SLOT(zoomIn()));

    connect(ui->btn_zoom_out,
            SIGNAL(clicked(bool)),
            this,
            SLOT(zoomOut()));

    connect(ui->cmb_key,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateKey(int)));

    connect(ui->cmb_seq_len,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateSeqLength(int)));

    connect(ui->cmb_scale,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateScale(int)));

    connect(ui->cmb_back,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateBackgroundSeq(int)));
}

EditFrame::~EditFrame()
{
    delete ui;
}

void EditFrame::updateSeqName()
{
    m_seq->set_name(ui->txt_seq_name->document()->toPlainText().toStdString());
}

void EditFrame::updateGridSnap(int newSnap)
{
    //add one as the UI elements are indexed
    //from zero
//    m_snap = newSnap + 1;
//    m_seqroll_wid->set_snap(m_snap);
//    m_seqevent_wid->set_snap(newSnap);
//    m_seq->set_snap_tick(newSnap);

}

void EditFrame::updateMidiBus(int newIndex)
{

}

void EditFrame::updateMidiChannel(int newIndex)
{
    m_seq->set_midi_channel(newIndex + 1);
}

void EditFrame::undo()
{

}

void EditFrame::redo()
{

}

void EditFrame::showTools()
{

}

void EditFrame::updateNoteLength(int newIndex)
{

}

void EditFrame::zoomIn()
{

}

void EditFrame::zoomOut()
{

}

void EditFrame::updateKey(int newIndex)
{

}

void EditFrame::updateSeqLength(int newIndex)
{

}

void EditFrame::updateScale(int newIndex)
{

}

void EditFrame::updateBackgroundSeq(int newIndex)
{

}
