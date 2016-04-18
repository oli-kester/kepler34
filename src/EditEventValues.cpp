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

    //this is perhaps the background?

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

        if ( tick >= start_tick && tick <= end_tick )
        {

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

//            m_painter->drawText(m_numbers[event_height],
//                                0,0,
//                                event_x + 3,
//                                c_dataarea_y - 25,
//                                6,30);
        }
    }

    //draw numbers

//    for( int i=0; i<c_dataarea_y; ++i ){

//        m_numbers[i] = Gdk::Pixmap::create( m_window,
//                                            6,
//                                            30, -1  );

//        m_gc->set_foreground( m_white );
//        m_numbers[i]->draw_rectangle(m_gc,true,
//                                     0,
//                                     0,
//                                     6,
//                                     30 );

//        char val[5];
//        snprintf(val, sizeof(val), "%3d\n", i);
//        char num[6];
//        memset( num, 0, 6);
//        num[0] = val[0];
//        num[2] = val[1];
//        num[4] = val[2];

//        p_font_renderer->render_string_on_drawable(m_gc,
//                                                   0,
//                                                   0,
//                                                   m_numbers[i], &num[0], font::BLACK );
//        p_font_renderer->render_string_on_drawable(m_gc,
//                                                   0,
//                                                   8,
//                                                   m_numbers[i], &num[2], font::BLACK );
//        p_font_renderer->render_string_on_drawable(m_gc,
//                                                   0,
//                                                   16,
//                                                   m_numbers[i], &num[4], font::BLACK );

//    }

//    //draw selection

//    int x,y,w,h;
//    m_gc->set_foreground( m_black );
//    m_gc->set_line_attributes( 1,
//                               Gdk::LINE_SOLID,
//                               Gdk::CAP_NOT_LAST,
//                               Gdk::JOIN_MITER );

//    /* replace old */
//    m_window->draw_drawable(m_gc,
//                            m_pixmap,
//                            m_old.x,
//                            m_old.y,
//                            m_old.x,
//                            m_old.y,
//                            m_old.width + 1,
//                            m_old.height + 1 );

//    xy_to_rect ( m_drop_x,
//                 m_drop_y,
//                 m_current_x,
//                 m_current_y,
//                 &x, &y,
//                 &w, &h );

//    x -= m_scroll_offset_x;

//    m_old.x = x;
//    m_old.y = y;
//    m_old.width = w;
//    m_old.height = h;

//    m_gc->set_foreground(m_black);
//    m_window->draw_line(m_gc,
//                        m_current_x - m_scroll_offset_x,
//                        m_current_y,
//                        m_drop_x - m_scroll_offset_x,
//                        m_drop_y );



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

void EditEventValues::xy_to_rect(  int a_x1,  int a_y1,
                                   int a_x2,  int a_y2,
                                   int *a_x,  int *a_y,
                                   int *a_w,  int *a_h )
{
    /* checks mins / maxes..  the fills in x,y
       and width and height */

    if ( a_x1 < a_x2 ){
        *a_x = a_x1;
        *a_w = a_x2 - a_x1;
    } else {
        *a_x = a_x2;
        *a_w = a_x1 - a_x2;
    }

    if ( a_y1 < a_y2 ){
        *a_y = a_y1;
        *a_h = a_y2 - a_y1;
    } else {
        *a_y = a_y2;
        *a_h = a_y1 - a_y2;
    }
}

