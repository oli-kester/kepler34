#include "LiveFrame.hpp"
#include "Globals.hpp"
#include "ui_LiveFrame.h"

LiveFrame::LiveFrame(QWidget *parent, MidiPerformance *perf) :
    QFrame(parent),
    ui(new Ui::LiveFrame),
    m_main_perf(perf),
    mCanAddNew(false)
{
    setSizePolicy(QSizePolicy::Expanding,
                  QSizePolicy::Expanding);

    setFocusPolicy(Qt::StrongFocus);

    ui->setupUi(this);

    mMsgBoxNewSeqCheck = new QMessageBox(this);
    mMsgBoxNewSeqCheck->setText(tr("Sequence already present"));
    mMsgBoxNewSeqCheck->setInformativeText(tr("There is already a sequence stored in this slot. Overwrite it and create a new blank sequence?"));
    mMsgBoxNewSeqCheck->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mMsgBoxNewSeqCheck->setDefaultButton(QMessageBox::No);

    setBank(0);

    connect(ui->spinBank,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(updateBank(int)));
    
    connect(ui->txtBankName,
            SIGNAL(textChanged()),
            this,
            SLOT(updateBankName()));

    //start refresh timer to queue regular redraws
    mRedrawTimer = new QTimer(this);
    mRedrawTimer->setInterval(50);
    connect(mRedrawTimer,
            SIGNAL(timeout()),
            this,
            SLOT(update()));
    mRedrawTimer->start();

}

void LiveFrame::paintEvent(QPaintEvent *)
{
    drawAllSequences();
}

LiveFrame::~LiveFrame()
{
    delete ui,
            mMsgBoxNewSeqCheck,
            mRedrawTimer;
}

void LiveFrame::drawSequence(int a_seq)
{
    mPainter = new QPainter(this);
    mPen = new QPen(Qt::black);
    mBrush = new QBrush(Qt::darkGray);
    mFont.setPointSize(6);
    mFont.setLetterSpacing(QFont::AbsoluteSpacing,
                           1);
    mPainter->setPen(*mPen);
    mPainter->setBrush(*mBrush);
    mPainter->setFont(mFont);

    //grab frame dimensions for scaled drawing
    thumbW = (ui->frame->width() - c_mainwid_spacing * 8)
            / c_mainwnd_cols;
    thumbH = (ui->frame->height() - c_mainwid_spacing * 5)
            / c_mainwnd_rows;

    previewW = thumbW - mFont.pointSize() * 2;
    previewH = thumbH - mFont.pointSize() * 5;

    if ( a_seq >= (m_bank_id  * c_mainwnd_rows * c_mainwnd_cols ) &&
         a_seq <  ((m_bank_id + 1)  * c_mainwnd_rows * c_mainwnd_cols ))
    {
        int i =  (a_seq / c_mainwnd_rows) % c_mainwnd_cols;
        int j =  a_seq % c_mainwnd_rows;
        
        int base_x = (ui->frame->x() +
                      (thumbW + c_mainwid_spacing) * i);
        int base_y = (ui->frame->y() +
                      (thumbH + c_mainwid_spacing) * j);
        
        //draw outline of this seq thumbnail
        mPainter->drawRect(base_x,
                           base_y,
                           thumbW,
                           thumbH);
        
        if (m_main_perf->is_active(a_seq))
        {
            MidiSequence *seq = m_main_perf->get_sequence(a_seq);
            
            //set foreground/background colours based on seq's play state
            if (seq->get_playing())
            {
                m_last_playing[a_seq] = true;
                mBrush->setColor(Qt::yellow);
                mPen->setColor(Qt::black);
            }
            else
            {
                m_last_playing[a_seq] = false;
                mBrush->setColor(Qt::lightGray);
                mPen->setColor(Qt::black);
            }
            
            //draw background of thumbnail
            mPainter->setBrush(*mBrush);
            mPainter->setPen(*mPen);
            mPainter->drawRect(base_x + 1,
                               base_y + 1,
                               thumbW - 2,
                               thumbH - 2);
            
            //write seq data strings on thumbnail
            mPainter->setPen(*mPen);
            char name[20];
            snprintf(name, sizeof name, "%.13s", seq->get_name());
            
            mPainter->drawText(base_x + c_text_x,
                               base_y + 4,
                               80,
                               80,
                               1,
                               name);

            /* midi channel + key + timesig */
            char str[20];

            if (m_main_perf->show_ui_sequence_key())
            {
                snprintf( str, sizeof str, "%c", (char)m_main_perf->lookup_keyevent_key( a_seq ) );

                mPainter->drawText(base_x + thumbW - 7,
                                   base_y + thumbH * 4 - 2,
                                   str);
            }

            snprintf( str, sizeof str,
                      "%d-%d %ld/%ld",
                      seq->get_midi_bus(),
                      seq->get_midi_channel()+1,
                      seq->getBeatsPerMeasure(), seq->getBeatWidth());

            mPainter->drawText(base_x + 5,
                               base_y + thumbH - 5,
                               str);

            int rectangle_x = base_x + 7;
            int rectangle_y = base_y + 15;

            if (seq->get_queued()){

                mPen->setColor(Qt::green);
                mPainter->setPen(*mPen);
                mPainter->drawRect(rectangle_x - 2,
                                   rectangle_y - 1,
                                   thumbW * 0.9,
                                   thumbH * 0.7);
            }

            mPen->setColor(Qt::gray);
            mBrush->setStyle(Qt::NoBrush);
            mPainter->setBrush(*mBrush);
            mPainter->setPen(*mPen);
            mPainter->drawRect(rectangle_x - 2,
                               rectangle_y - 1,
                               previewW,
                               previewH);

            int lowest_note = seq->get_lowest_note_event( );
            int highest_note = seq->get_highest_note_event( );

            int height = highest_note - lowest_note;
            height += 2;

            int length = seq->getLength();

            long tick_s;
            long tick_f;
            int note;

            bool selected;

            int velocity;
            draw_type dt;

            seq->reset_draw_marker();

            //add padding to box measurements
            previewH -= 6;
            previewW -= 6;
            rectangle_x += 2;
            rectangle_y += 2;

            while ( (dt = seq->get_next_note_event( &tick_s, &tick_f, &note,
                                                    &selected, &velocity )) != DRAW_FIN ){

                int note_y = previewH -
                        (previewH  * (note + 1 - lowest_note)) / height ;

                int tick_s_x = (tick_s * previewW)  / length;
                int tick_f_x = (tick_f * previewH)  / length;

                if ( dt == DRAW_NOTE_ON || dt == DRAW_NOTE_OFF )
                    tick_f_x = tick_s_x + 1;
                if ( tick_f_x <= tick_s_x )
                    tick_f_x = tick_s_x + 1;

                //draw line representing this note
                mPen->setColor(Qt::black);
                mPen->setWidth(2);
                mPainter->setPen(*mPen);
                mPainter->drawLine(rectangle_x + tick_s_x,
                                   rectangle_y + note_y,
                                   rectangle_x + tick_f_x,
                                   rectangle_y + note_y );
            }

            //draw playhead
            int a_tick = m_main_perf->get_tick();
            a_tick += (length - seq->get_trigger_offset( ));
            a_tick %= length;

            long tick_x = a_tick * previewW / length;

            mPen->setColor(Qt::red);

            if ( seq->get_queued()){
                mPen->setColor(Qt::green);
            }

            mPen->setWidth(1);
            mPainter->setPen(*mPen);
            mPainter->drawLine(rectangle_x + tick_x - 1,
                               rectangle_y - 1,
                               rectangle_x + tick_x - 1,
                               rectangle_y + previewH + 1);


        }
        else
        {
            //no sequence present. Insert placeholder
            mPen->setColor(Qt::black);
            mFont.setPointSize(15);
            mPainter->setPen(*mPen);
            mPainter->setFont(mFont);
            mPainter->drawText(base_x + 2,
                               base_y + 17,
                               "+");
        }
    }
    delete mPainter, mPen, mBrush;
}
void LiveFrame::drawAllSequences()
{
    for (int i=0; i < (c_mainwnd_rows * c_mainwnd_cols); i++){
        drawSequence(i + (m_bank_id * c_mainwnd_rows * c_mainwnd_cols));

        m_last_tick_x[i + (m_bank_id * c_mainwnd_rows * c_mainwnd_cols)] = 0;
    }
}

void LiveFrame::setBank(int newBank)
{
    m_bank_id = newBank;

    if (m_bank_id < 0)
        m_bank_id = c_max_sets - 1;

    if (m_bank_id >= c_max_sets)
        m_bank_id = 0;

    m_main_perf->set_offset(m_bank_id);

    QString bankName = (*m_main_perf->getBankName(m_bank_id)).c_str();
    ui->txtBankName->setPlainText(bankName);

    ui->spinBank->setValue(m_bank_id);

    update();

    qDebug() << "Newly selected bank" << endl
             << "Name - " << bankName << endl
             << "ID - " << m_bank_id << endl;

}

void LiveFrame::redraw()
{
    drawAllSequences();
}

void LiveFrame::updateBank(int newBank)
{
    m_main_perf->setBank(newBank);
    setBank(newBank);
    m_main_perf->setModified(true);
}

void LiveFrame::updateBankName()
{
    updateInternalBankName();
    m_main_perf->setModified(true);
}

void LiveFrame::updateInternalBankName()
{
    string newName =
            ui->txtBankName->document()->toPlainText().toStdString();

    qDebug() << "LiveFrame.cpp, New bank name is - "
             << QString(newName.c_str()) << endl;

    m_main_perf->setBankName(m_bank_id, &newName);
}

int LiveFrame::seqIDFromClickXY(int click_x, int click_y)
{
    /* adjust for border */
    int x = click_x - c_mainwid_border;
    int y = click_y - c_mainwid_border;

    /* is it in the box ? */
    if ( x < 0
         || x >= ((thumbW + c_mainwid_spacing ) * c_mainwnd_cols )
         || y < 0
         || y >= ((thumbH + c_mainwid_spacing ) * c_mainwnd_rows )){

        return -1;
    }

    /* gives us in box corrdinates */
    int box_test_x = x % (thumbW + c_mainwid_spacing);
    int box_test_y = y % (thumbH + c_mainwid_spacing);

    /* right inactive side of area */
    if ( box_test_x > thumbW
         || box_test_y > thumbH ){

        return -1;
    }

    x /= (thumbW + c_mainwid_spacing);
    y /= (thumbH + c_mainwid_spacing);

    int seqId =  ( (x * c_mainwnd_rows + y)
                   + ( m_bank_id * c_mainwnd_rows * c_mainwnd_cols ));

    return seqId;

}

void LiveFrame::mousePressEvent(QMouseEvent *event)
{
    mCurrentSeq = seqIDFromClickXY(
                event->x(), event->y());

    if ( mCurrentSeq != -1
         && event->button() == Qt::LeftButton )
    {
        mButtonDown = true;
    }
}

void LiveFrame::mouseReleaseEvent(QMouseEvent *event)
{
    /* get the sequence number we clicked on */
    mCurrentSeq = seqIDFromClickXY(
                event->x(), event->y());

    mButtonDown = false;

    /* if we're on a valid sequence, hit the left mouse button,
     * and are not dragging a sequence - toggle playing*/
    if ( mCurrentSeq != -1
         && event->button() == Qt::LeftButton
         && !mMoving )
    {
        if ( m_main_perf->is_active( mCurrentSeq ))
        {
            m_main_perf->sequence_playing_toggle( mCurrentSeq );
            update();
        }
        else
        {
            mCanAddNew = true;
        }
    }

    /* if left mouse button & we're moving a seq between slots */
    if (event->button() == Qt::LeftButton
            && mMoving )
    {
        mMoving = false;

        if (!m_main_perf->is_active(mCurrentSeq)
                && mCurrentSeq != -1
                && !m_main_perf->is_sequence_in_edit(mCurrentSeq))
        {
            m_main_perf->new_sequence(mCurrentSeq);
            *(m_main_perf->get_sequence( mCurrentSeq )) = m_moving_seq;

            update();

        }
        else
        {
            m_main_perf->new_sequence( mOldSeq  );
            *(m_main_perf->get_sequence( mOldSeq )) = m_moving_seq;

            update();
        }
    }

    /* check for right mouse click - this launches the popup menu */
    if (mCurrentSeq != -1
            && event->button() == Qt::RightButton)
    {
        mPopup = new QMenu(this);

        QAction *actionNew = new QAction("New sequence", mPopup);
        mPopup->addAction(actionNew);
        QObject::connect(actionNew,
                         SIGNAL(triggered(bool)),
                         this,
                         SLOT(newSeq()));

        if (m_main_perf->is_active(mCurrentSeq))
        {
            QAction *actionEdit = new QAction("Edit sequence", mPopup);
            mPopup->addAction(actionEdit);
            QObject::connect(actionEdit,
                             SIGNAL(triggered(bool)),
                             this,
                             SLOT(editSeq()));
        }

        mPopup->exec(QCursor::pos());
    }

    //middle button launches seq editor
    if (mCurrentSeq != -1
            && event->button() == Qt::MiddleButton
            && m_main_perf->is_active(mCurrentSeq))
    {
        callEditor(m_main_perf->get_sequence(mCurrentSeq));
    }
}

void LiveFrame::mouseMoveEvent(QMouseEvent *event)
{
    int seqId = seqIDFromClickXY(event->x(), event->y());

    if ( mButtonDown )
    {
        if (seqId != mCurrentSeq
                && !mMoving
                && !m_main_perf->is_sequence_in_edit(mCurrentSeq)){

            /* lets drag a sequence between slots */
            if ( m_main_perf->is_active( mCurrentSeq )){

                mOldSeq = mCurrentSeq;
                mMoving = true;

                /* save the sequence and clear the old slot */
                m_moving_seq = *(m_main_perf->get_sequence( mCurrentSeq ));
                m_main_perf->delete_sequence( mCurrentSeq );

                update();
            }
        }
    }
}

void LiveFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mCanAddNew)
    {
        newSeq();
        mCanAddNew = false;
    }

}

void LiveFrame::newSeq()
{
    if (m_main_perf->is_active(mCurrentSeq))
    {
        int choice = mMsgBoxNewSeqCheck->exec();
        if (choice == QMessageBox::No)
            return;
    }
    m_main_perf->new_sequence(mCurrentSeq);
    m_main_perf->get_sequence( mCurrentSeq )->set_dirty();
    //TODO reenable - disabled opening the editor for each new seq
    //    callEditor(m_main_perf->get_sequence(m_current_seq));

}

void LiveFrame::editSeq()
{
    callEditor(m_main_perf->get_sequence(mCurrentSeq));
}

void LiveFrame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_BracketLeft:
        setBank(m_bank_id - 1);
        break;
    case Qt::Key_BracketRight:
        setBank(m_bank_id + 1);
        break;
    default: //sequence mute toggling
        quint32 keycode =  event->key();
        qDebug() << keycode << endl;
        if (m_main_perf->get_key_events()->count(event->key()) != 0)
            sequence_key(m_main_perf->lookup_keyevent_seq(event->key()));
        break;
    }
}

void LiveFrame::keyReleaseEvent(QKeyEvent *event)
{

}

void LiveFrame::sequence_key( int a_seq )
{
    /* add screen set offset */
    a_seq += m_main_perf->getBank() * c_mainwnd_rows * c_mainwnd_cols;

    if ( m_main_perf->is_active( a_seq ) ){

        m_main_perf->sequence_playing_toggle( a_seq );
    }
}
