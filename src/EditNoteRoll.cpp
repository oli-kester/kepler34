#include "EditNoteRoll.hpp"

EditNoteRoll::EditNoteRoll(MidiPerformance *a_perf,
                           MidiSequence *a_seq,
                           QWidget *parent):
    QWidget(parent),
    m_perform(a_perf),
    m_seq(a_seq),
    m_scale(0),
    m_key(0),
    m_zoom(12),
    m_size_x(5000),
    m_size_y(c_num_keys),
    m_snap(1),
    m_old_progress_x(0),
    m_background_sequence(0),
    m_drawing_background_seq(false)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);

    //start refresh timer to queue regular redraws
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    m_timer->start();

}

void EditNoteRoll::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_brush = new QBrush(Qt::NoBrush);
    m_pen = new QPen(Qt::black);
    m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setBrush(*m_brush);
    m_painter->setFont(m_font);

    //draw background

    /* clear background */
    //    m_pen->setColor(Qt::white);
    //    m_rect = QRect(0,
    //                   0,
    //                   m_window_x,
    //                   m_window_y);
    //    m_painter->drawRect(m_rect);

    m_pen->setColor(Qt::gray);
    m_pen->setStyle(Qt::SolidLine);
    m_painter->setPen(*m_pen);

    //for each note row in our grid
    for ( int i=0; i <= c_num_keys; i++ )
    {
        //set line colour dependent on the note row we're on
        //        if (0 == (((c_num_keys - i) + ( 12 - m_key )) % 12))
        //        {
        //            m_pen->setColor(Qt::darkGray);
        //            m_pen->setStyle(Qt::SolidLine);
        //            m_painter->setPen(*m_pen);
        //        }
        //        else if (11 == (((c_num_keys - i) + ( 12 - m_key )) % 12))
        //        {
        //            m_pen->setColor(Qt::darkGray);
        //            m_pen->setStyle(Qt::SolidLine);
        //            m_painter->setPen(*m_pen);
        //        }

        //draw horizontal grid lines
        m_painter->drawLine(0,
                            i * c_key_y,
                            m_size_x,
                            i * c_key_y);

        if ( m_scale != c_scale_off ){

            if ( !c_scales_policy[m_scale][ ((c_num_keys - i) - 1 + ( 12 - m_key )) % 12] )
                m_painter->drawRect(0,
                                    i * c_key_y + 1,
                                    m_size_x,
                                    c_key_y - 1);
        }
    }

    int measures_per_line = 1;
    int ticks_per_measure =  m_seq->get_bpm() * (4 * c_ppqn) / m_seq->get_bw();
    int ticks_per_beat =  (4 * c_ppqn) / m_seq->get_bw();
    int ticks_per_step = 6 * m_zoom;
    int ticks_per_m_line =  ticks_per_measure * measures_per_line;

    m_pen->setColor(Qt::gray);

    for ( int i = 0; i < m_size_x; i += ticks_per_step )
    {
        int base_line = i / m_zoom;

        if ( i % ticks_per_m_line == 0 ){

            /* solid line on every beat */
            m_pen->setColor(Qt::black);
            m_pen->setStyle(Qt::SolidLine);

        }
        else if (i % ticks_per_beat == 0 )
        {

            m_pen->setColor(Qt::darkGray);
            m_pen->setStyle(Qt::SolidLine);
        }
        else
        {
            m_pen->setStyle(Qt::DashLine);

            int i_snap = i - (i % m_snap);

            if( i == i_snap ){
                m_pen->setColor(Qt::darkGray);
            } else {
                m_pen->setColor(Qt::gray);
            }
        }

        //draw vertical grid lines
        m_painter->setPen(*m_pen);
        m_painter->drawLine(base_line,
                            0,
                            base_line,
                            m_size_y * 8);
    }
    /* reset line style */
    m_pen->setColor(Qt::black);
    m_pen->setStyle(Qt::SolidLine);

    //draw playhead
    m_painter->setPen(*m_pen);
    m_painter->drawLine(m_old_progress_x,
                        0,
                        m_old_progress_x,
                        m_size_y * 8);

    m_old_progress_x = ((m_seq->get_last_tick() * 3) / m_zoom);

    if ( m_old_progress_x != 0 ){

        m_pen->setColor(Qt::black);
        m_painter->setPen(*m_pen);
        m_painter->drawLine(m_old_progress_x,
                            0,
                            m_old_progress_x,
                            m_size_y);
    }

    //draw events
    long tick_s;
    long tick_f;
    int note;

    int note_x;
    int note_width;
    int note_y;
    int note_height;

    bool selected;

    int velocity;

    draw_type dt;

    int start_tick = 0;
    int end_tick = (m_size_x * m_zoom);

    MidiSequence *seq = NULL;

    for( int method=0; method<2; ++method )	{

        if ( method == 0 && m_drawing_background_seq  ){

            if ( m_perform->is_active( m_background_sequence )){
                seq = m_perform->get_sequence( m_background_sequence );
            } else {
                method++;
            }
        } else if ( method == 0 ){
            method++;
        }

        if ( method==1){
            seq = m_seq;
        }

        /* draw boxes from sequence */
        m_pen->setColor(Qt::black);
        m_pen->setStyle(Qt::SolidLine);
        seq->reset_draw_marker();

        while ( (dt = seq->get_next_note_event( &tick_s, &tick_f, &note, &selected, &velocity )) != DRAW_FIN ) {

            if ((tick_s >= start_tick && tick_s <= end_tick) ||
                    ( (dt == DRAW_NORMAL_LINKED) && (tick_f >= start_tick && tick_f <= end_tick))
                    ) {

                /* turn into screen corrids */
                note_x = tick_s / m_zoom;
                note_y = c_rollarea_y -(note * c_key_y) - c_key_y - 1 + 2;
                note_height = c_key_y - 3;

                int in_shift = 0;
                int length_add = 0;

                if ( dt == DRAW_NORMAL_LINKED )
                {
                    if (tick_f >= tick_s) {
                        note_width = (tick_f - tick_s) / m_zoom;
                        if ( note_width < 1 ) note_width = 1;
                    } else {
                        note_width = (m_seq->get_length() - tick_s) / m_zoom;
                    }

                } else {
                    note_width = 16 / m_zoom;
                }

                if ( dt == DRAW_NOTE_ON )
                {
                    in_shift = 0;
                    length_add = 2;
                }

                if ( dt == DRAW_NOTE_OFF )
                {
                    in_shift = -1;
                    length_add = 1;
                }

                m_pen->setColor(Qt::black);

                if ( method == 0 )
                    m_pen->setColor(Qt::darkGray);

                //draw outer note boundary
                m_brush->setStyle(Qt::SolidPattern);
                m_brush->setColor(Qt::black);
                m_painter->setBrush(*m_brush);
                m_painter->setPen(*m_pen);
                m_painter->drawRect(note_x,
                                    note_y,
                                    note_width,
                                    note_height);

                if (tick_f < tick_s) {

                    //                    m_painter->setPen(*m_pen);
                    //                    m_painter->drawRect(0,
                    //                                        note_y,
                    //                                        tick_f / m_zoom,
                    //                                        note_height);
                }

                /* draw inside box if there is room */
                if ( note_width > 3 ){

                    if ( selected )
                        m_brush->setColor(Qt::red);
                    else
                        m_brush->setColor(Qt::white);

                    m_painter->setBrush(*m_brush);
                    if ( method == 1 ) {
                        if (tick_f >= tick_s) {
                            m_painter->drawRect(note_x + in_shift,
                                                note_y,
                                                note_width - 1 + length_add,
                                                note_height - 1 );
                        } else {
                            m_painter->drawRect(note_x + in_shift,
                                                note_y,
                                                note_width ,
                                                note_height - 1);

                            m_painter->drawRect(0,
                                                note_y,
                                                (tick_f/m_zoom) - 3 + length_add,
                                                note_height - 1);
                        }
                    }
                }
            }
        }
    }
}

void EditNoteRoll::mousePressEvent(QMouseEvent *event)
{

}

void EditNoteRoll::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditNoteRoll::mouseMoveEvent(QMouseEvent *event)
{

}

QSize EditNoteRoll::sizeHint() const
{
    return QSize(m_size_x * 0.1, m_size_y * 8);
}
