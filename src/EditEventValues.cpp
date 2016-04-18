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

    //edit note velocity for now
    m_status = EVENT_NOTE_ON;
    m_cc = 1;

    m_old = new QRect();

    //start refresh timer to queue regular redraws
    mTimer = new QTimer(this);
    mTimer->setInterval(20);
    QObject::connect(mTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    mTimer->start();
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
    return QSize(m_seq->getLength() / m_zoom + 100, c_dataarea_y);
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

            for( int i = 0; i < c_dataarea_y; ++i ){

                //                m_numbers[i] = Gdk::Pixmap::create( m_window,
                //                                                    6,
                //                                                    30, -1  );

                //                m_gc->set_foreground( m_white );
                //                m_numbers[i]->draw_rectangle(m_gc,true,
                //                                             0,
                //                                             0,
                //                                             6,
                //                                             30 );

                char val[5];
                snprintf(val, sizeof(val), "%3d\n", i);
                char num[6];
                memset( num, 0, 6);
                num[0] = val[0];
                num[2] = val[1];
                num[4] = val[2];

                //                m_pen->setColor(Qt::black);
                //                m_painter->setPen(*m_pen);
                //                p_font_renderer->render_string_on_drawable(m_gc,
                //                                                           0,
                //                                                           0,
                //                                                           m_numbers[i], &num[0], font::BLACK );
                //                p_font_renderer->render_string_on_drawable(m_gc,
                //                                                           0,
                //                                                           8,
                //                                                           m_numbers[i], &num[2], font::BLACK );
                //                p_font_renderer->render_string_on_drawable(m_gc,
                //                                                           0,
                //                                                           16,
                //                                                           m_numbers[i], &num[4], font::BLACK );

            }


            //                        m_painter->drawText(m_numbers[event_height],
            //                                            0,0,
            //                                            event_x + 3,
            //                                            c_dataarea_y - 25,
            //                                            6,30);
        }
    }

    //draw edit line

    int x,y,w,h;
    m_pen->setColor(Qt::black);
    m_painter->setPen(*m_pen);

    xy_to_rect ( m_drop_x,
                 m_drop_y,
                 m_current_x,
                 m_current_y,
                 &x, &y,
                 &w, &h );

    m_old->setX(x);
    m_old->setY(y);
    m_old->setWidth(w);
    m_old->setHeight(h);

    m_painter->drawLine(m_current_x,
                        m_current_y,
                        m_drop_x,
                        m_drop_y );

    delete m_painter;
    delete m_brush;
    delete m_pen;
}

void EditEventValues::mousePressEvent(QMouseEvent *event)
{
    m_seq->push_undo();

    /* set values for line */
    m_drop_x = (int) event->x();
    m_drop_y = (int) event->y();

    /* reset box that holds dirty redraw spot */
    m_old->setX(0);
    m_old->setY(0);
    m_old->setWidth(0);
    m_old->setHeight(0);

    m_dragging = true;
}

void EditEventValues::mouseReleaseEvent(QMouseEvent *event)
{
    m_current_x = (int) event->x();
    m_current_y = (int) event->y();

    if ( m_dragging ){

        long tick_s, tick_f;

        if ( m_current_x < m_drop_x ){
            swap( m_current_x, m_drop_x );
            swap( m_current_y, m_drop_y );
        }

        convert_x( m_drop_x, &tick_s );
        convert_x( m_current_x, &tick_f );

        m_seq->change_event_data_range( tick_s, tick_f,
                                        m_status,
                                        m_cc,
                                        c_dataarea_y - m_drop_y -1,
                                        c_dataarea_y - m_current_y-1 );

        /* convert x,y to ticks, then set events in range */
        m_dragging = false;

    }
}

void EditEventValues::mouseMoveEvent(QMouseEvent *event)
{
    if ( m_dragging ){

        m_current_x = (int) event->x();
        m_current_y = (int) event->y();

        long tick_s, tick_f;

        int adj_x_min, adj_x_max,
                adj_y_min, adj_y_max;

        if ( m_current_x < m_drop_x ){

            adj_x_min = m_current_x;
            adj_y_min = m_current_y;
            adj_x_max = m_drop_x;
            adj_y_max = m_drop_y;

        } else {

            adj_x_max = m_current_x;
            adj_y_max = m_current_y;
            adj_x_min = m_drop_x;
            adj_y_min = m_drop_y;
        }

        convert_x( adj_x_min, &tick_s );
        convert_x( adj_x_max, &tick_f );

        m_seq->change_event_data_range( tick_s, tick_f,
                                        m_status,
                                        m_cc,
                                        c_dataarea_y - adj_y_min -1,
                                        c_dataarea_y - adj_y_max -1 );
    }
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

void EditEventValues::set_data_type( unsigned char a_status, unsigned char a_control = 0  )
{
    m_status = a_status;
    m_cc = a_control;
}

void EditEventValues::convert_x( int a_x, long *a_tick )
{
    *a_tick = a_x * m_zoom;
}

