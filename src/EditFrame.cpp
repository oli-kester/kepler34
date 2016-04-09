#include "EditFrame.hpp"
#include "ui_EditFrame.h"

EditFrame::EditFrame(QWidget *parent, MidiPerformance *perf, MidiSequence *seq) :
    QFrame(parent),
    mPerformance(perf),
    mSeq(seq),
    ui(new Ui::EditFrame)
{
    ui->setupUi(this);

    setSizePolicy(QSizePolicy::Expanding,
                  QSizePolicy::Expanding);

    /* fill options for grid snap & note length
     * combo box and set their defaults */

    //16th intervals
    for (int i = 0; i < 8; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->cmb_grid_snap->insertItem(i, combo_text);
        ui->cmb_note_len->insertItem(i, combo_text);
    }

    ui->cmb_grid_snap->insertSeparator(8);
    ui->cmb_note_len->insertSeparator(8);

    //triplet intervals
    for (int i = 1; i < 8; i++)
    {
        QString combo_text = "1/" +
                QString::number(pow(2, i) * 1.5);
        ui->cmb_grid_snap->insertItem(i + 9, combo_text);
        ui->cmb_note_len->insertItem(i + 9, combo_text);
    }
    ui->cmb_grid_snap->setCurrentIndex(3);
    ui->cmb_note_len->setCurrentIndex(3);

    /* fill options for MIDI channel numbers */
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

    /* pull data from sequence object */
    ui->txt_seq_name->setPlainText(mSeq->get_name());
    ui->cmb_midi_chan->setCurrentIndex(mSeq->get_midi_channel());

    QString snapText("1/");
    snapText.append(QString::number(c_ppqn * 4 / mSeq->getSnap_tick()));
    ui->cmb_grid_snap->setCurrentText(snapText);

    QString noteLenText("1/16");
    ui->cmb_note_len->setCurrentText(noteLenText);

    /* set out our custom elements */
    mSeq->set_editing(true);

    m_scroll_area = new QScrollArea(this);
    ui->vbox_centre->addWidget(m_scroll_area);

    mContainer = new QWidget(m_scroll_area);
    m_layout_grid = new QGridLayout(mContainer);
    mContainer->setLayout(m_layout_grid);

    m_palette = new QPalette();
    m_palette->setColor(QPalette::Background, Qt::darkGray);
    mContainer->setPalette(*m_palette);

    mKeyboard = new EditKeys(mSeq, mContainer);
    mTimeBar = new EditTimeBar(mSeq, mContainer);
    mNoteGrid = new EditNoteRoll(mPerformance, mSeq, mContainer);

    m_layout_grid->setSpacing(0);
    m_layout_grid->addWidget(mKeyboard, 1, 0, 1, 1);
    m_layout_grid->addWidget(mTimeBar, 0, 1, 1, 1);
    m_layout_grid->addWidget(mNoteGrid, 1, 1, 1, 1);
    m_layout_grid->setAlignment(mNoteGrid, Qt::AlignTop);

    m_scroll_area->setWidget(mContainer);

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
            SLOT(updateSeqLength()));

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
    mSeq->set_name(ui->txt_seq_name->document()->toPlainText().toStdString());
}

void EditFrame::updateGridSnap(int snapIndex)
{
    int snap;
    switch (snapIndex)
    {
    case 0:
        snap = c_ppqn * 4;
        break;
    case 1:
        snap = c_ppqn * 2;
        break;
    case 2:
        snap = c_ppqn * 1;
        break;
    case 3:
        snap = c_ppqn / 2;
        break;
    case 4:
        snap = c_ppqn / 4;
        break;
    case 5:
        snap = c_ppqn / 8;
        break;
    case 6:
        snap = c_ppqn / 16;
        break;
    case 7:
        snap = c_ppqn / 32;
        break;
        //ignore index 8 as it's a separator
    case 9:
        snap = c_ppqn * 4  / 3;
        break;
    case 10:
        snap = c_ppqn * 2  / 3;
        break;
    case 11:
        snap = c_ppqn * 1 / 3;
        break;
    case 12:
        snap = c_ppqn / 2 / 3;
        break;
    case 13:
        snap = c_ppqn / 4 / 3;
        break;
    case 14:
        snap = c_ppqn / 8 / 3;
        break;
    case 15:
        snap = c_ppqn / 16 / 3;
        break;
    }

    mNoteGrid->set_snap(snap);
    mSeq->set_snap_tick(snap);

}

void EditFrame::updateMidiBus(int newIndex)
{

}

void EditFrame::updateMidiChannel(int newIndex)
{
    mSeq->set_midi_channel(newIndex + 1);
}

void EditFrame::undo()
{
    mSeq->pop_undo();
}

void EditFrame::redo()
{
    mSeq->pop_redo();
}

void EditFrame::showTools()
{

}

void EditFrame::updateNoteLength(int newIndex)
{
    int length;
    switch (newIndex)
    {
    case 0:
        length = c_ppqn * 4;
        break;
    case 1:
        length = c_ppqn * 2;
        break;
    case 2:
        length = c_ppqn * 1;
        break;
    case 3:
        length = c_ppqn / 2;
        break;
    case 4:
        length = c_ppqn / 4;
        break;
    case 5:
        length = c_ppqn / 8;
        break;
    case 6:
        length = c_ppqn / 16;
        break;
    case 7:
        length = c_ppqn / 32;
        break;
        //ignore index 8 as it's a separator
    case 9:
        length = c_ppqn * 4  / 3;
        break;
    case 10:
        length = c_ppqn * 2  / 3;
        break;
    case 11:
        length = c_ppqn * 1 / 3;
        break;
    case 12:
        length = c_ppqn / 2 / 3;
        break;
    case 13:
        length = c_ppqn / 4 / 3;
        break;
    case 14:
        length = c_ppqn / 8 / 3;
        break;
    case 15:
        length = c_ppqn / 16 / 3;
        break;
    }

    mNoteGrid->setNote_length(length);
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

void EditFrame::updateSeqLength()
{
    int measures = ui->cmb_seq_len->currentText().toInt();
    mSeq->setNumMeasures(measures);
    mTimeBar->updateGeometry();
    mNoteGrid->updateGeometry();
    mContainer->adjustSize();
}

void EditFrame::updateScale(int newIndex)
{

}

void EditFrame::updateBackgroundSeq(int newIndex)
{

}
