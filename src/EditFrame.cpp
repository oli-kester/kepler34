#include "EditFrame.hpp"
#include "ui_EditFrame.h"

EditFrame::EditFrame(QWidget *parent,
                     MidiPerformance *perf,
                     int seqId):
    QFrame(parent),
    ui(new Ui::EditFrame),
    mSeq(perf->get_sequence(seqId)),
    mPerformance(perf),
    mSeqId(seqId)
{
    ui->setupUi(this);

    editMode = mPerformance->getEditMode(seqId);

    setSizePolicy(QSizePolicy::Expanding,
                  QSizePolicy::Expanding);

    /* fill options for grid snap & note length
     * combo box and set their defaults */

    //16th intervals
    for (int i = 0; i < 8; i++)
    {
        QString combo_text = "1/" + QString::number(pow(2,i));
        ui->cmbGridSnap->insertItem(i, combo_text);
        ui->cmbNoteLen->insertItem(i, combo_text);
    }

    ui->cmbGridSnap->insertSeparator(8);
    ui->cmbNoteLen->insertSeparator(8);

    //triplet intervals
    for (int i = 1; i < 8; i++)
    {
        QString combo_text = "1/" +
                QString::number(pow(2, i) * 1.5);
        ui->cmbGridSnap->insertItem(i + 9, combo_text);
        ui->cmbNoteLen->insertItem(i + 9, combo_text);
    }
    ui->cmbGridSnap->setCurrentIndex(3);
    ui->cmbNoteLen->setCurrentIndex(3);

    /* fill options for MIDI channel numbers */
    for (int i = 0; i <= 15; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmbMidiChan->insertItem(i,combo_text);
    }

    // fill options for seq length
    for (int i = 0; i <= 15; i++)
    {
        QString combo_text = QString::number(i+1);
        ui->cmbSeqLen->insertItem(i,combo_text);
    }
    ui->cmbSeqLen->insertItem(16,"32");
    ui->cmbSeqLen->insertItem(17,"64");

    // fill options for scale
    ui->cmbScale->insertItem(0,"Off");
    ui->cmbScale->insertItem(1,"Major");
    ui->cmbScale->insertItem(2,"Minor");

    /* pull data from sequence object */
    ui->txtSeqName->setPlainText(mSeq->get_name());
    ui->cmbMidiChan->setCurrentIndex(mSeq->get_midi_channel());

    QString snapText("1/");
    snapText.append(QString::number(c_ppqn * 4 / mSeq->getSnap_tick()));
    ui->cmbGridSnap->setCurrentText(snapText);

    QString seqLenText(QString::number(mSeq->getNumMeasures()));
    ui->cmbSeqLen->setCurrentText(seqLenText);

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
    mNoteGrid->updateEditMode(editMode);
    mEventValues = new EditEventValues(mSeq, mContainer);
    mEventTriggers = new EditEventTriggers(mSeq, mEventValues, mContainer);

    m_layout_grid->setSpacing(0);
    m_layout_grid->addWidget(mKeyboard, 1, 0, 1, 1);
    m_layout_grid->addWidget(mTimeBar, 0, 1, 1, 1);
    m_layout_grid->addWidget(mNoteGrid, 1, 1, 1, 1);
    m_layout_grid->addWidget(mEventTriggers, 2, 1, 1, 1);
    m_layout_grid->addWidget(mEventValues, 3, 1, 1, 1);
    m_layout_grid->setAlignment(mNoteGrid, Qt::AlignTop);

    m_scroll_area->setWidget(mContainer);

    ui->cmbMidiBus->hide();
    ui->lblMidiBus->hide();
    ui->btnTools->hide();
    ui->btnPlay->hide();
    ui->btnThru->hide();
    ui->btnQRec->hide();
    ui->btnRec->hide();
    ui->lblBackgroundSeq->hide();
    ui->cmbBackgroundSeq->hide();
    ui->lblRecVol->hide();
    ui->cmbRecVol->hide();
    ui->lblEventSelect->hide();
    ui->cmbEventSelect->hide();
    ui->lblKey->hide();
    ui->cmbKey->hide();
    ui->lblScale->hide();
    ui->cmbScale->hide();

    //connect all the UI signals
    connect(ui->txtSeqName,
            SIGNAL(textChanged()),
            this,
            SLOT(updateSeqName()));

    connect(ui->cmbGridSnap,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateGridSnap(int)));

    connect(ui->cmbMidiBus,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateMidiBus(int)));

    connect(ui->cmbMidiChan,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateMidiChannel(int)));

    connect(ui->btnUndo,
            SIGNAL(clicked(bool)),
            this,
            SLOT(undo()));

    connect(ui->btnRedo,
            SIGNAL(clicked(bool)),
            this,
            SLOT(redo()));

    connect(ui->btnTools,
            SIGNAL(clicked(bool)),
            this,
            SLOT(showTools()));

    connect(ui->cmbNoteLen,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateNoteLength(int)));

    connect(ui->btnZoomIn,
            SIGNAL(clicked(bool)),
            this,
            SLOT(zoomIn()));

    connect(ui->btnZoomOut,
            SIGNAL(clicked(bool)),
            this,
            SLOT(zoomOut()));

    connect(ui->cmbKey,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateKey(int)));

    connect(ui->cmbSeqLen,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateSeqLength()));

    connect(ui->cmbScale,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateScale(int)));

    connect(ui->cmbBackgroundSeq,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateBackgroundSeq(int)));

    connect(ui->btnDrum,
            SIGNAL(clicked(bool)),
            this,
            SLOT(toggleEditorMode()));
}

EditFrame::~EditFrame()
{
    delete ui;
}

void EditFrame::updateSeqName()
{
    mSeq->set_name(ui->txtSeqName->document()->toPlainText().toStdString());
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
    mSeq->set_midi_channel(newIndex);
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
    mNoteGrid->zoomIn();
    mTimeBar->zoomIn();
    mEventTriggers->zoomIn();
    mEventValues->zoomIn();
    updateDrawGeometry();
}

void EditFrame::zoomOut()
{
    mNoteGrid->zoomOut();
    mTimeBar->zoomOut();
    mEventTriggers->zoomOut();
    mEventValues->zoomOut();
    updateDrawGeometry();
}

void EditFrame::updateKey(int newIndex)
{

}

void EditFrame::updateSeqLength()
{
    int measures = ui->cmbSeqLen->currentText().toInt();
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

void EditFrame::updateDrawGeometry()
{
    QString seqLenText(QString::number(mSeq->getNumMeasures()));
    ui->cmbSeqLen->setCurrentText(seqLenText);
    mTimeBar->updateGeometry();
    mNoteGrid->updateGeometry();
    mContainer->adjustSize();
}

void EditFrame::toggleEditorMode()
{
    switch (editMode)
    {
    case NOTE:
        editMode = DRUM;
        ui->cmbNoteLen->hide();
        ui->lblNoteLen->hide();
        break;

    case DRUM:
        editMode = NOTE;
        ui->cmbNoteLen->show();
        ui->lblNoteLen->show();
        break;
    }

    mPerformance->setEditMode(mSeqId, editMode);
    mNoteGrid->updateEditMode(editMode);
}

void EditFrame::setEditorMode(edit_mode_e mode)
{
    editMode = mode;
    mPerformance->setEditMode(mSeqId, editMode);
    mNoteGrid->updateEditMode(mode);
}
