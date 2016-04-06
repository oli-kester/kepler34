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
    m_drawing_background_seq(false),
    m_selecting(false),
    m_adding(false),
    m_moving(false),
    m_moving_init(false),
    m_growing(false),
    m_painting(false),
    m_paste(false),
    m_is_drag_pasting(false),
    m_is_drag_pasting_start(false),
    m_justselected_one(false)
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

            if ( i == i_snap )
            {
                m_pen->setColor(Qt::darkGray);
            }
            else
            {
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

    if ( m_old_progress_x != 0 )
    {
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

    for( int method = 0; method < 2; ++method )	{

        if ( method == 0 && m_drawing_background_seq  ){

            if ( m_perform->is_active( m_background_sequence ))
            {
                seq = m_perform->get_sequence( m_background_sequence );
            }
            else
            {
                method++;
            }
        }

        else if ( method == 0 )
        {
            method++;
        }

        if ( method==1)
        {
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
                        if ( note_width < 1 )
                            note_width = 1;
                    }
                    else
                    {
                        note_width = (m_seq->get_length() - tick_s) / m_zoom;
                    }

                }
                else
                {
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

                //draw outer note boundary (shadow)
                m_brush->setStyle(Qt::SolidPattern);
                m_brush->setColor(Qt::black);
                m_painter->setBrush(*m_brush);
                m_painter->setPen(*m_pen);
                m_painter->drawRect(note_x,
                                    note_y,
                                    note_width,
                                    note_height);

                if (tick_f < tick_s)
                {
                    //                    m_painter->setPen(*m_pen);
                    //                    m_painter->drawRect(0,
                    //                                        note_y,
                    //                                        tick_f / m_zoom,
                    //                                        note_height);
                }

                /* draw inside box if there is room */
                if ( note_width > 3 )
                {
                    if ( selected )
                        m_brush->setColor(Qt::red);
                    else
                        m_brush->setColor(Qt::white);

                    m_painter->setBrush(*m_brush);
                    if ( method == 1 )
                    {
                        if (tick_f >= tick_s)
                        {
                            //draw inner note (highlight)
                            m_painter->drawRect(note_x + in_shift,
                                                note_y,
                                                note_width - 1 + length_add,
                                                note_height - 1 );
                        }
                        else
                        {
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
    int numsel;

    long tick_s;
    long tick_f;
    int note;
    int note_l;

    bool needs_update = false;

    int norm_x, norm_y, snapped_x, snapped_y;

    snapped_x = norm_x = event->x();
    snapped_y = norm_y = event->y();

    snap_x( &snapped_x );
    snap_y( &snapped_y );

    /* y is always snapped */
    m_current_y = m_drop_y = snapped_y;

    if ( m_paste ){

        convert_xy( snapped_x, snapped_y, &tick_s, &note );
        m_paste = false;
        m_seq->push_undo();
        m_seq->paste_selected( tick_s, note );

        needs_update = true;

    } else {

        /*  left mouse button     */
        if ( event->button() == Qt::LeftButton )
        {
            /* selection, normal x */
            m_current_x = m_drop_x = norm_x;

            /* turn x,y in to tick/note */
            convert_xy( m_drop_x, m_drop_y, &tick_s, &note );

            if ( m_adding )
            {
                /* start the paint job */
                m_painting = true;

                /* adding, snapped x */
                m_current_x = m_drop_x = snapped_x;
                convert_xy( m_drop_x, m_drop_y, &tick_s, &note );

                // test if a note is already there
                // fake select, if so, no add
                if ( ! m_seq->select_note_events( tick_s, note,
                                                  tick_s, note,
                                                  MidiSequence::e_would_select ))
                {

                    /* add note, length = little less than snap */
                    m_seq->push_undo();
//                    m_seq->add_note( tick_s, m_note_length - 2, note, true );
                    m_seq->add_note(tick_s, 100, note, true );

                    needs_update = true;
                }

            }
            else /* we're selecting */
            {


                if ( !m_seq->select_note_events( tick_s, note,
                                                 tick_s, note,
                                                 MidiSequence::e_is_selected ))
                {
                    if ( ! (event->modifiers()) & Qt::ControlModifier)
                    {
                        m_seq->unselect();
                    }


                    /* on direct click select only one event */
                    numsel = m_seq->select_note_events( tick_s,note,tick_s,note,
                                                        MidiSequence::e_select_one );

                    /* none selected, start selection box */
                    if ( numsel == 0 )
                    {
                        if ( event->button() == Qt::LeftButton )
                            m_selecting = true;
                    }
                    else
                    {
                        needs_update = true;
                    }
                }


                if ( m_seq->select_note_events(tick_s, note,
                                               tick_s, note,
                                               MidiSequence::e_is_selected ))
                {
                    // moving - left click only
                    if ( event->button() == Qt::LeftButton && !(event->modifiers()) & Qt::ControlModifier)
                    {
                        m_moving_init = true;
                        needs_update = true;


                        /* get the box that selected elements are in */
                        m_seq->get_selected_box( &tick_s, &note,
                                                 &tick_f, &note_l );


                        //                        convert_tn_box_to_rect( tick_s, tick_f, note_h, note_l,
                        //                                                &m_selected.x,
                        //                                                &m_selected.y,
                        //                                                &m_selected.width,
                        //                                                &m_selected.height );

                        /* save offset that we get from the snap above */
                        //                        int adjusted_selected_x = m_selected.x;
                        //                        snap_x( &adjusted_selected_x );
                        //                        m_move_snap_offset_x = ( m_selected.x - adjusted_selected_x);

                        /* align selection for drawing */
                        //                        snap_x( &m_selected.x );

                        m_current_x = m_drop_x = snapped_x;
                    }

                    /* middle mouse button, or left-ctrl click (for 2button mice) */
                    if (event->button() == Qt::MiddleButton ||
                            (event->button() == Qt::LeftButton &&
                             (event->modifiers() & Qt::ControlModifier)) ){

                        /* moving, normal x */
                        //m_current_x = m_drop_x = norm_x;
                        //convert_xy( m_drop_x, m_drop_y, &tick_s, &note_h );

                        m_growing = true;

                        /* get the box that selected elements are in */
                        m_seq->get_selected_box( &tick_s, &note,
                                                 &tick_f, &note_l );

                        //                        convert_tn_box_to_rect( tick_s, tick_f, note_h, note_l,
                        //                                                &m_selected.x,
                        //                                                &m_selected.y,
                        //                                                &m_selected.width,
                        //                                                &m_selected.height );

                    }
                }
            }
        }

        /*     right mouse button      */
        if ( event->button() == Qt::RightButton )
        {
            set_adding(true);
        }

    }

    /* if they clicked, something changed */
    if ( needs_update )
    {
        m_seq->set_dirty();
    }
}

void EditNoteRoll::mouseReleaseEvent(QMouseEvent *event)
{
    long tick_s;
    long tick_f;
    int note_h;
    int note_l;
    int x,y,w,h;

    bool needs_update = false;

    m_current_x = event->x();
    m_current_y = event->y();;

    snap_y ( &m_current_y );

    if ( m_moving )
        snap_x( &m_current_x );

    int delta_x = m_current_x - m_drop_x;
    int delta_y = m_current_y - m_drop_y;

    long delta_tick;
    int delta_note;

    if ( event->button() == Qt::LeftButton ){

        if ( m_selecting ){

            //            xy_to_rect ( m_drop_x,
            //                             m_drop_y,
            //                             m_current_x,
            //                             m_current_y,
            //                             &x, &y,
            //                             &w, &h );

            convert_xy( x,     y, &tick_s, &note_h );
            convert_xy( x+w, y+h, &tick_f, &note_l );

            m_seq->select_note_events( tick_s, note_h, tick_f, note_l, MidiSequence::e_select );

            needs_update = true;
        }

        if (  m_moving  ){

            /* adjust for snap */
            delta_x -= m_move_snap_offset_x;

            /* convert deltas into screen corridinates */
            convert_xy( delta_x, delta_y, &delta_tick, &delta_note );

            /* since delta_note was from delta_y, it will be filpped
               ( delta_y[0] = note[127], etc.,so we have to adjust */
            delta_note = delta_note - (c_num_keys-1);

            m_seq->push_undo();
            m_seq->move_selected_notes( delta_tick, delta_note );
            needs_update = true;
        }

    }

    if (event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton){

        if ( m_growing ){

            /* convert deltas into screen corridinates */
            convert_xy( delta_x, delta_y, &delta_tick, &delta_note );
            m_seq->push_undo();

            if ( event->modifiers() & Qt::ShiftModifier )
            {
                m_seq->stretch_selected( delta_tick );
            }
            else
            {
                m_seq->grow_selected( delta_tick );
            }

            needs_update = true;
        }
    }

    if ( event->button() == Qt::RightButton ){
        set_adding(false);
    }

    /* turn off */
    m_selecting = false;
    m_moving = false;
    m_growing = false;
    m_paste = false;
    m_moving_init = false;
    m_painting = false;

    m_seq->unpaint_all();

    /* if they clicked, something changed */
    if (  needs_update )
    {
        m_seq->set_dirty();
    }
}

void EditNoteRoll::mouseMoveEvent(QMouseEvent *event)
{
    m_current_x = event->x();
    m_current_y = event->y();

    int note;
    long tick;

    if ( m_moving_init )
    {
        m_moving_init = false;
        m_moving = true;
    }


    snap_y( &m_current_y );
    convert_xy( 0, m_current_y, &tick, &note );

    //    m_seqkeys_wid->set_hint_key( note );

    if ( m_selecting || m_moving || m_growing || m_paste ){

        if ( m_moving || m_paste ){
            snap_x( &m_current_x );
        }

        //        draw_selection_on_window();

    }

    if ( m_painting )
    {
        snap_x( &m_current_x );
        convert_xy( m_current_x, m_current_y, &tick, &note );

        m_seq->add_note( tick, m_note_length - 2, note, true );
    }
}

void EditNoteRoll::keyPressEvent(QKeyEvent *event)
{

}

void EditNoteRoll::keyReleaseEvent(QKeyEvent *event)
{

}

QSize EditNoteRoll::sizeHint() const
{
    return QSize(m_size_x * 0.1, m_size_y * 8);
}

void EditNoteRoll::snap_y( int *a_y )
{
    *a_y = *a_y - (*a_y % c_key_y);
}

void EditNoteRoll::snap_x( int *a_x )
{
    //snap = number pulses to snap to
    //m_zoom = number of pulses per pixel
    //so snap / m_zoom  = number pixels to snap to
    int mod = (m_snap / m_zoom);
    if ( mod <= 0 )
        mod = 1;

    *a_x = *a_x - (*a_x % mod );
}

void EditNoteRoll::convert_xy( int a_x, int a_y, long *a_tick, int *a_note)
{
    *a_tick = a_x * m_zoom;
    *a_note = (c_rollarea_y - a_y - 2) / c_key_y;
}

void EditNoteRoll::set_adding(bool a_adding)
{
    if ( a_adding )
    {
        setCursor(Qt::PointingHandCursor);

        m_adding = true;

    }
    else
    {
        setCursor(Qt::ArrowCursor);

        m_adding = false;
    }
}

void EditNoteRoll::set_snap(int newSnapIndex)
{
    m_snap = newSnapIndex;
}
