#include "LiveFrame.hpp"
#include "Globals.hpp"
#include "ui_LiveFrame.h"

LiveFrame::LiveFrame(QWidget *parent, MidiPerformance *perf) :
    QFrame(parent),
    ui(new Ui::LiveFrame),
    m_main_perf(perf)
{
    ui->setupUi(this);

    m_screenset = 0;
}

void LiveFrame::paintEvent(QPaintEvent *)
{
    drawSequence(0);
}

LiveFrame::~LiveFrame()
{
    delete ui;
}

void LiveFrame::drawSequence(int a_seq)
{
    if ( a_seq >= (m_screenset  * c_mainwnd_rows * c_mainwnd_cols ) &&
         a_seq <  ((m_screenset + 1)  * c_mainwnd_rows * c_mainwnd_cols )){

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

        //TODO add dummy seq0 to get this to trigger
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
            m_painter->setPen(*m_foreground);
            m_painter->drawRect(*rect);
        }

    }
}
