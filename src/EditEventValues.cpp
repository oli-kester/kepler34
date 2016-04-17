#include "EditEventValues.hpp"

EditEventValues::EditEventValues(MidiSequence *a_seq,
                                 QWidget *parent):
    QWidget(parent),
    m_seq(a_seq),
    m_zoom(1),
    m_dragging(false)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);
}

void EditEventValues::zoomIn()
{
    if (m_zoom > 1)
        m_zoom *= 0.5;
}

void EditEventValues::zoomOut()
{
    if (m_zoom < 32)
        m_zoom *= 2;
}

QSize EditEventValues::sizeHint() const
{
    return QSize(m_seq->getLength() / m_zoom + 100, 175);
}

void EditEventValues::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::black);
    m_brush = new QBrush(Qt::lightGray, Qt::SolidPattern);
    m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setBrush(*m_brush);
    m_painter->setFont(m_font);

    long tick;

    unsigned char d0,d1;

    int event_x;
    int event_height;

    bool selected;

    int start_tick = 0 ;
    int end_tick = (width() * m_zoom);

    m_painter->drawRect(0,
                        0,
                        width() - 1,
                        height() - 1);


    m_seq->reset_draw_marker();
    while ( m_seq->get_next_event( m_status,
                                   m_cc,
                                   &tick, &d0, &d1,
                                   &selected ) == true )
    {

        if ( tick >= start_tick && tick <= end_tick ){

            /* turn into screen corrids */

            event_x = tick / m_zoom;

            /* generate the value */
            event_height = d1;

            if ( m_status == EVENT_PROGRAM_CHANGE ||
                 m_status == EVENT_CHANNEL_PRESSURE  ){

                event_height = d0;
            }

            /* draw vert lines */
            m_painter->drawLine(event_x + 1,
                                height() - event_height,
                                event_x + 1,
                                height());

//            a_draw->draw_drawable(m_gc,
//                                  m_numbers[event_height],
//                                  0,0,
//                                  event_x + 3 - m_scroll_offset_x,
//                                  c_dataarea_y - 25,
//                                  6,30);
        }
    }

    delete m_painter;
    delete m_brush;
    delete m_pen;
}

void EditEventValues::mousePressEvent(QMouseEvent *event)
{

}

void EditEventValues::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditEventValues::mouseMoveEvent(QMouseEvent *event)
{

}
