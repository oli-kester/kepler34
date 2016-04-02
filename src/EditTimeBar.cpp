#include "EditTimeBar.hpp"

EditTimeBar::EditTimeBar(MidiSequence *a_seq, QWidget *parent):
    QWidget(parent),

    m_seq(a_seq),

    m_scroll_offset_ticks(0),
    m_scroll_offset_x(0)
{
    //start refresh timer to queue regular redraws
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    m_timer->start();

    m_zoom = 5;
    m_window_x = 800;
    m_window_y = 700;
}

void EditTimeBar::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::white);
    m_painter->setPen(*m_pen);

    m_rect = QRect(0,
                   0,
                   m_window_x,
                   m_window_y);
    m_painter->drawRect(m_rect);

    m_pen->setColor(Qt::black);
    m_rect.setRect(0,
                   m_window_y - 1,
                   m_window_x,
                   m_window_y - 1);
    m_painter->drawRect(m_rect);

    int measure_length_32nds =  m_seq->get_bpm() * 32 /
            m_seq->get_bw();

    //printf ( "measure_length_32nds[%d]\n", measure_length_32nds );

    int measures_per_line = (128 / measure_length_32nds) / (32 / m_zoom);
    if ( measures_per_line <= 0 )
        measures_per_line = 1;

    //printf( "measures_per_line[%d]\n", measures_per_line );

    int ticks_per_measure =  m_seq->get_bpm() * (4 * c_ppqn) / m_seq->get_bw();
    int ticks_per_step =  ticks_per_measure * measures_per_line;
    int start_tick = m_scroll_offset_ticks - (m_scroll_offset_ticks % ticks_per_step );
    int end_tick = (m_window_x * m_zoom) + m_scroll_offset_ticks;

    //printf ( "ticks_per_step[%d] start_tick[%d] end_tick[%d]\n",
    //         ticks_per_step, start_tick, end_tick );

    /* draw vert lines */
    m_pen->setColor(Qt::black);
    for (int i = start_tick; i < end_tick; i += ticks_per_step)
    {
        int base_line = i / m_zoom;

        /* beat */
        m_line = QLine(base_line - m_scroll_offset_x,
                       0,
                       base_line - m_scroll_offset_x,
                       m_window_y);
        m_painter->drawLine(m_line);


        char bar[5];
        snprintf(bar, sizeof(bar), "%d", (i/ ticks_per_measure ) + 1);

        m_pen->setColor(Qt::black);
        m_rect.setRect(base_line + 2 - m_scroll_offset_x,
                       0,
                       100,
                       100);
        m_painter->drawText(m_rect, bar);

    }

    long end_x = m_seq->get_length() / m_zoom - m_scroll_offset_x;

    m_pen->setColor(Qt::black);
    m_rect.setRect(end_x,
                   9,
                   19,
                   8);
    m_painter->drawRect(m_rect);
    m_rect.setRect(end_x + 1,
                   9,
                   19,
                   8);

    m_pen->setColor(Qt::white);
    m_painter->drawText(m_rect,
                        "END");


}

void EditTimeBar::mousePressEvent(QMouseEvent *event)
{

}

void EditTimeBar::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditTimeBar::mouseMoveEvent(QMouseEvent *event)
{

}
