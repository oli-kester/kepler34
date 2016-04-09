#include "LiveFrame.hpp"
#include "Globals.hpp"
#include "ui_LiveFrame.h"

LiveFrame::LiveFrame(QWidget *parent, MidiPerformance *perf) :
    QFrame(parent),
    ui(new Ui::LiveFrame),
    m_main_perf(perf)
{
    setFocusPolicy(Qt::StrongFocus);

    ui->setupUi(this);

    setBank(0);

    QObject::connect(ui->spinBank,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(updateBank(int)));
    
    QObject::connect(ui->txtBankName,
                     SIGNAL(textChanged()),
                     this,
                     SLOT(updateBankName()));
}

void LiveFrame::paintEvent(QPaintEvent *)
{
    drawAllSequences();
}

LiveFrame::~LiveFrame()
{
    delete ui;
}

void LiveFrame::drawSequence(int a_seq)
{
    if ( a_seq >= (m_bank_id  * c_mainwnd_rows * c_mainwnd_cols ) &&
         a_seq <  ((m_bank_id + 1)  * c_mainwnd_rows * c_mainwnd_cols ))
    {
        int i =  (a_seq / c_mainwnd_rows) % c_mainwnd_cols;
        int j =  a_seq % c_mainwnd_rows;
        
        int base_x = (c_mainwid_border +
                      (c_seqarea_x + c_mainwid_spacing) * i);
        int base_y = (c_mainwid_border +
                      (c_seqarea_y + c_mainwid_spacing) * j);
        
        //draw outline of this seq thumbnail
        m_painter = new QPainter(this);
        rect = new QRect(base_x,
                         base_y,
                         c_seqarea_x,
                         c_seqarea_y);
        m_painter->drawRect(*rect);
        
        if (m_main_perf->is_active(a_seq))
        {
            MidiSequence *seq = m_main_perf->get_sequence(a_seq);
            
            //set foreground/background colours based on seq's play state
            if (seq->get_playing())
            {
                m_last_playing[a_seq] = true;
                m_background = new QPen(Qt::black);
                m_foreground = new QPen(Qt::white);
            }
            else
            {
                m_last_playing[a_seq] = false;
                m_background = new QPen(Qt::white);
                m_foreground = new QPen(Qt::black);
            }
            
            //draw background of thumbnail
            rect = new QRect(base_x + 1,
                             base_y + 1,
                             c_seqarea_x - 2,
                             c_seqarea_y - 2 );
            m_painter->setPen(*m_background);
            m_painter->drawRect(*rect);
            
            //write seq data strings on thumbnail
            m_painter->setPen(*m_foreground);
            char name[20];
            snprintf( name, sizeof name, "%.13s", seq->get_name() );
            
            m_painter->drawText(base_x + c_text_x,
                                base_y + 4,
                                80,
                                80,
                                1,
                                name);
        }
    }
    update();
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
    
    redraw();
    
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
    bankNameUpdateInternal();
    m_main_perf->setModified(true);
}

void LiveFrame::bankNameUpdateInternal()
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
         || x >= ((c_seqarea_x + c_mainwid_spacing ) * c_mainwnd_cols )
         || y < 0
         || y >= ((c_seqarea_y + c_mainwid_spacing ) * c_mainwnd_rows )){

        return -1;
    }

    /* gives us in box corrdinates */
    int box_test_x = x % (c_seqarea_x + c_mainwid_spacing);
    int box_test_y = y % (c_seqarea_y + c_mainwid_spacing);

    /* right inactive side of area */
    if ( box_test_x > c_seqarea_x
         || box_test_y > c_seqarea_y ){

        return -1;
    }

    x /= (c_seqarea_x + c_mainwid_spacing);
    y /= (c_seqarea_y + c_mainwid_spacing);

    int seqId =  ( (x * c_mainwnd_rows + y)
                   + ( m_bank_id * c_mainwnd_rows * c_mainwnd_cols ));

    return seqId;

}

void LiveFrame::mousePressEvent(QMouseEvent *event)
{
    m_current_seq = seqIDFromClickXY(
                event->x(), event->y());

    if ( m_current_seq != -1
         && event->button() == Qt::LeftButton )
    {
        m_button_down = true;
    }
}

void LiveFrame::mouseReleaseEvent(QMouseEvent *event)
{
    /* get the sequence number we clicked on */
    m_current_seq = seqIDFromClickXY(
                event->x(), event->y());

    m_button_down = false;

    /* if we're on a valid sequence, hit the left mouse button,
     * and are not dragging a sequence - toggle playing*/
    if ( m_current_seq != -1
         && event->button() == Qt::LeftButton
         && !m_moving ){

        if ( m_main_perf->is_active( m_current_seq )){

            m_main_perf->sequence_playing_toggle( m_current_seq );
        }
        else
        {
            newSeq();
        }

        update();
    }

    /* if left mouse button & we're moving a seq between slots */
    if (event->button() == Qt::LeftButton
            && m_moving )
    {
        m_moving = false;

        if (!m_main_perf->is_active(m_current_seq)
                && m_current_seq != -1
                && !m_main_perf->is_sequence_in_edit(m_current_seq))
        {
            m_main_perf->new_sequence(m_current_seq);
            *(m_main_perf->get_sequence( m_current_seq )) = m_moving_seq;

            update();

        }
        else
        {
            m_main_perf->new_sequence( m_old_seq  );
            *(m_main_perf->get_sequence( m_old_seq )) = m_moving_seq;

            update();
        }
    }

    /* check for right mouse click - this launches the popup menu */
    if (m_current_seq != -1
            && event->button() == Qt::RightButton)
    {
        m_popup = new QMenu(this);

        QAction *actionNew = new QAction("New sequence", m_popup);
        m_popup->addAction(actionNew);
        QObject::connect(actionNew,
                         SIGNAL(triggered(bool)),
                         this,
                         SLOT(newSeq()));

        if (m_main_perf->is_active(m_current_seq))
        {
            QAction *actionEdit = new QAction("Edit sequence", m_popup);
            m_popup->addAction(actionEdit);
            QObject::connect(actionEdit,
                             SIGNAL(triggered(bool)),
                             this,
                             SLOT(editSeq()));

        }

        m_popup->exec(QCursor::pos());
    }
}

void LiveFrame::mouseMoveEvent(QMouseEvent *event)
{
    int seqId = seqIDFromClickXY(event->x(), event->y());

    if ( m_button_down )
    {
        if (seqId != m_current_seq
                && !m_moving
                && !m_main_perf->is_sequence_in_edit(m_current_seq)){

            /* lets drag a sequence between slots */
            if ( m_main_perf->is_active( m_current_seq )){

                m_old_seq = m_current_seq;
                m_moving = true;

                /* save the sequence and clear the old slot */
                m_moving_seq = *(m_main_perf->get_sequence( m_current_seq ));
                m_main_perf->delete_sequence( m_current_seq );

                update();
            }
        }
    }
}

void LiveFrame::newSeq()
{
    //TODO if this is already a sequence,
    //check that we want to replace it
    m_main_perf->new_sequence(m_current_seq);
    m_main_perf->get_sequence( m_current_seq )->set_dirty();
    callEditor(m_main_perf->get_sequence(m_current_seq));

}

void LiveFrame::editSeq()
{
    callEditor(m_main_perf->get_sequence(m_current_seq));
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
        if (m_main_perf->get_key_events()->count( event->nativeScanCode()) != 0)
            sequence_key(m_main_perf->lookup_keyevent_seq(event->nativeScanCode()));
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
