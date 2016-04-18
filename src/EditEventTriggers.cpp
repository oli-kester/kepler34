#include "EditEventTriggers.hpp"

EditEventTriggers::EditEventTriggers(MidiSequence *a_seq,
                                     EditEventValues *a_seqdata_wid,
                                     QWidget *parent):
    QWidget(parent),
    m_seq(a_seq),
    m_seqdata_wid(a_seqdata_wid),
    m_zoom(1),
    m_snap(1),
    m_selecting(false),
    m_moving_init(false),
    m_moving(false),
    m_growing(false),
    m_painting(false),
    m_paste(false),
    m_status(EVENT_NOTE_ON)

{

}

void EditEventTriggers::zoomIn()
{
    if (m_zoom > 1)
        m_zoom *= 0.5;
}

void EditEventTriggers::zoomOut()
{
    if (m_zoom < 32)
        m_zoom *= 2;
}

QSize EditEventTriggers::sizeHint() const
{
    return QSize(m_seq->getLength() / m_zoom + 100, c_eventarea_y + 1);
}

void EditEventTriggers::paintEvent(QPaintEvent *)
{
    mPainter = new QPainter(this);
    mPen = new QPen(Qt::black);
    mBrush = new QBrush(Qt::darkGray, Qt::SolidPattern);
    mFont.setPointSize(6);
    mPainter->setPen(*mPen);
    mPainter->setBrush(*mBrush);
    mPainter->setFont(mFont);

    /* draw background */
    mPainter->drawRect(0,
                       0,
                       width(),
                       height());

    int measures_per_line = 1;
    int ticks_per_measure =  m_seq->getBeatsPerMeasure() * (4 * c_ppqn) / m_seq->getBeatWidth();
    int ticks_per_beat =  (4 * c_ppqn) / m_seq->getBeatWidth();
    int ticks_per_step = 6 * m_zoom;
    int ticks_per_m_line =  ticks_per_measure * measures_per_line;
    int start_tick = 0;
    int end_tick = width() * m_zoom;

    //printf ( "ticks_per_step[%d] start_tick[%d] end_tick[%d]\n",
    //         ticks_per_step, start_tick, end_tick );

    for ( int i = start_tick; i < width(); i += ticks_per_step )
    {
        int base_line = i;

        if ( i % ticks_per_m_line == 0 )
        {
            /* solid line on every beat */
            mPen->setColor(Qt::black);
            mPen->setStyle(Qt::SolidLine);

        }
        else if (i % ticks_per_beat == 0 )
        {
            mPen->setStyle(Qt::SolidLine);
            mPen->setColor(Qt::black);
        }
        else
        {
            mPen->setColor(Qt::lightGray);
            mPen->setStyle(Qt::DashLine);
        }

        mPainter->setPen(*mPen);
        mPainter->drawLine(base_line,
                           1,
                           base_line,
                           c_eventarea_y);
    }

    //draw event boxes

    long tick;
    int x;
    unsigned char d0,d1;
    bool selected;

    /* draw boxes from sequence */
    mPen->setColor(Qt::black);
    mPen->setStyle(Qt::SolidLine);

    m_seq->reset_draw_marker();
    while ( m_seq->get_next_event( m_status,
                                   m_cc,
                                   &tick, &d0, &d1,
                                   &selected ) == true ){
        if ( (tick >= start_tick && tick <= end_tick )){


            /* turn into screen corrids */
            x = tick / m_zoom;

            //draw outer note border
            mPen->setColor(Qt::black);
            mBrush->setStyle(Qt::SolidPattern);
            mBrush->setColor(Qt::black);
            mPainter->setBrush(*mBrush);
            mPainter->setPen(*mPen);
            mPainter->drawRect(x,
                               (c_eventarea_y - c_eventevent_y)/2,
                               c_eventevent_x,
                               c_eventevent_y );

            if ( selected )
                mBrush->setColor(Qt::red);
            else
                mBrush->setColor(Qt::white);

            //draw note highlight
            mPainter->setBrush(*mBrush);
            mPainter->drawRect(x,
                               (c_eventarea_y - c_eventevent_y)/2,
                               c_eventevent_x - 1,
                               c_eventevent_y - 1 );
        }
    }

    //draw selection

    int w;

    int y = (c_eventarea_y - c_eventevent_y)/2;
    int h =  c_eventevent_y;

    //painter reset
    mBrush->setStyle(Qt::NoBrush);
    mPainter->setBrush(*mBrush);

    if ( m_selecting ){

        x_to_w( m_drop_x, m_current_x, &x,&w );

        //        x -= m_scroll_offset_x;

        m_old->setX(x);
        m_old->setWidth(w);

        mPen->setColor(Qt::black);
        mPainter->setPen(*mPen);
        //        mPainter->drawRect(x,
        //                           y,
        //                           w,
        //                           h);
    }

    if ( m_moving || m_paste ){

        int delta_x = m_current_x - m_drop_x;

        x = m_selected->x() + delta_x;

        mPen->setColor(Qt::black);
        mPainter->setPen(*mPen);
        //        mPainter->drawRect(x,
        //                           y,
        //                           m_selected->width(),
        //                           h );
        m_old->setX(x);
        m_old->setWidth(m_selected->width());
    }

    delete mPainter;
    delete mBrush;
    delete mPen;
}

void EditEventTriggers::mousePressEvent(QMouseEvent *event)
{

}

void EditEventTriggers::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditEventTriggers::mouseMoveEvent(QMouseEvent *event)
{

}

void EditEventTriggers::x_to_w(int a_x1, int a_x2, int *a_x, int *a_w)
{
    if ( a_x1 < a_x2 ){
        *a_x = a_x1;
        *a_w = a_x2 - a_x1;
    } else {
        *a_x = a_x2;
        *a_w = a_x1 - a_x2;
    }
}
