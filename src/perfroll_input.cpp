#include "perform.h"
#include "perfroll_input.h"
#include "perfroll.h"

void FruityPerfInput::updateMousePtr( perfroll& ths )
{
    // context sensitive mouse
    long drop_tick;
    int drop_sequence;
    ths.convert_xy( m_current_x, m_current_y, &drop_tick, &drop_sequence );
    if (ths.m_mainperf->is_active( drop_sequence ))
    {
        long start, end;
        if (ths.m_mainperf->get_sequence(drop_sequence)->intersectTriggers( drop_tick, start, end ))
        {
            if (start <= drop_tick && drop_tick <= start + (c_perfroll_size_box_click_w * c_perf_scale_x) &&
                    (m_current_y % c_names_y) <= c_perfroll_size_box_click_w + 1)
            {
                ths.get_window()->set_cursor( Gdk::Cursor( Gdk::RIGHT_PTR ));
            }
            else if (end - (c_perfroll_size_box_click_w * c_perf_scale_x) <= drop_tick && drop_tick <= end &&
                    (m_current_y % c_names_y) >= c_names_y - c_perfroll_size_box_click_w - 1)
            {
                ths.get_window()->set_cursor( Gdk::Cursor( Gdk::LEFT_PTR ));
            }
            else
            {
                ths.get_window()->set_cursor( Gdk::Cursor( Gdk::CENTER_PTR ));
            }
        }
        else
        {
            ths.get_window()->set_cursor( Gdk::Cursor( Gdk::PENCIL ));
        }
    }
    else
    {
        ths.get_window()->set_cursor( Gdk::Cursor( Gdk::CROSSHAIR ));
    }
}

bool FruityPerfInput::on_button_press_event(GdkEventButton* a_ev, perfroll& ths)
{
    ths.grab_focus( );

    if ( ths.m_mainperf->is_active( ths.m_drop_sequence ))
    {
        ths.m_mainperf->get_sequence( ths.m_drop_sequence )->unselect_triggers( );
        ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
    }

    ths.m_drop_x = (int) a_ev->x;
    ths.m_drop_y = (int) a_ev->y;
    m_current_x = (int) a_ev->x;
    m_current_y = (int) a_ev->y;


    ths.convert_xy( ths.m_drop_x, ths.m_drop_y, &ths.m_drop_tick, &ths.m_drop_sequence );

    /*      left mouse button     */
    if ( a_ev->button == 1)
    {
        on_left_button_pressed(a_ev, ths);
    }

    /*     right mouse button      */
    if ( a_ev->button == 3 )
    {
        on_right_button_pressed(a_ev, ths);
    }

    /* left-ctrl, or middle: split */
    if ( a_ev->button == 2 )
    {
        if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

            bool state = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_trigger_state( ths.m_drop_tick );

            if ( state )
            {
                ths.split_trigger(ths.m_drop_sequence, ths.m_drop_tick);
            }
        }
    }

    updateMousePtr( ths );
    return true;
}

void FruityPerfInput::on_left_button_pressed(GdkEventButton* a_ev, perfroll& ths)
{
    if ( a_ev->state & GDK_CONTROL_MASK )
    {
        if ( ths.m_mainperf->is_active( ths.m_drop_sequence ))
        {
            bool state = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_trigger_state( ths.m_drop_tick );

            if ( state )
            {
                ths.split_trigger(ths.m_drop_sequence, ths.m_drop_tick);
            }
        }
    }
    else
    {
        long tick = ths.m_drop_tick;

        /* add a new note if we didnt select anything */
        //if (m_adding)
        {
            m_adding_pressed = true;

            if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){
                long seq_length = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_length( );

                bool state = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_trigger_state( tick );

                // resize the event, or move it, depending on where clicked.
                if ( state )
                {
                    //m_adding = false;
                    m_adding_pressed = false;
                    ths.m_mainperf->push_trigger_undo();
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )->select_trigger( tick );

                    long start_tick = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_selected_trigger_start_tick();
                    long end_tick = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_selected_trigger_end_tick();

                    if ( tick >= start_tick &&
                            tick <= start_tick + (c_perfroll_size_box_click_w * c_perf_scale_x) &&
                            (ths.m_drop_y % c_names_y) <= c_perfroll_size_box_click_w + 1 )
                    {
                        // clicked left side: begin a grow/shrink for the left side
                        ths.m_growing = true;
                        ths.m_grow_direction = true;
                        ths.m_drop_tick_trigger_offset = ths.m_drop_tick -
                            ths.m_mainperf->get_sequence( ths.m_drop_sequence )->
                            get_selected_trigger_start_tick( );
                    }
                    else
                        if ( tick >= end_tick - (c_perfroll_size_box_click_w * c_perf_scale_x) &&
                                tick <= end_tick &&
                                (ths.m_drop_y % c_names_y) >= c_names_y - c_perfroll_size_box_click_w - 1 )
                        {
                            // clicked right side: grow/shrink the right side
                            ths.m_growing = true;
                            ths.m_grow_direction = false;
                            ths.m_drop_tick_trigger_offset =
                                ths.m_drop_tick -
                                ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_selected_trigger_end_tick( );
                        }
                        else
                        {
                            // clicked in the middle - move it
                            ths.m_moving = true;
                            ths.m_drop_tick_trigger_offset = ths.m_drop_tick -
                                ths.m_mainperf->get_sequence( ths.m_drop_sequence )->
                                get_selected_trigger_start_tick( );

                        }

                    ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
                    ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
                    ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
                }

                // add an event:
                else
                {
                    // snap to length of sequence
                    tick = tick - (tick % seq_length);

                    ths.m_mainperf->push_trigger_undo();
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )->add_trigger( tick, seq_length );
                    ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
                    ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
                    ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);

                    //m_drop_tick_last = (m_drop_tick + seq_length - 1);
                }
            }
        }
    }
}

void FruityPerfInput::on_right_button_pressed(GdkEventButton* a_ev, perfroll& ths)
{
    //set_adding( false );

    long tick = ths.m_drop_tick;

    if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

        //long seq_length = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_length();

        bool state = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_trigger_state( tick );

        if ( state )
        {
            ths.m_mainperf->push_trigger_undo();
            ths.m_mainperf->get_sequence( ths.m_drop_sequence )->del_trigger( tick );
        }
    }
}

bool FruityPerfInput::on_button_release_event(GdkEventButton* a_ev, perfroll& ths)
{
    m_current_x = (int) a_ev->x;
    m_current_y = (int) a_ev->y;

    if ( a_ev->button == 1 || a_ev->button == 3 )
    {
        m_adding_pressed = false;
    }

    ths.m_moving = false;
    ths.m_growing = false;
    m_adding_pressed = false;

    if ( ths.m_mainperf->is_active( ths.m_drop_sequence  )){

        ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y );
    }

    updateMousePtr( ths );
    return true;
}

bool FruityPerfInput::on_motion_notify_event(GdkEventMotion* a_ev, perfroll& ths)
{
    long tick;
    int x = (int) a_ev->x;
    m_current_x = (int) a_ev->x;
    m_current_y = (int) a_ev->y;

    if (  m_adding_pressed ){

        ths.convert_x( x, &tick );

        if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

            long seq_length = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_length( );
            tick = tick - (tick % seq_length);

            /*long min_tick = (tick < m_drop_tick) ? tick : m_drop_tick;*/
            long length = seq_length;

            ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                ->grow_trigger( ths.m_drop_tick, tick, length);
            ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
        }
    }
    else if ( ths.m_moving || ths.m_growing )
    {
        if ( ths.m_mainperf->is_active( ths.m_drop_sequence))
        {
            ths.convert_x( x, &tick );
            tick -= ths.m_drop_tick_trigger_offset;

            tick = tick - tick % ths.m_snap;

            if ( ths.m_moving )
            {
                ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                    ->move_selected_triggers_to( tick, true );
            }
            if ( ths.m_growing )
            {
                if ( ths.m_grow_direction )
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                        ->move_selected_triggers_to( tick, false, 0 );
                else
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                        ->move_selected_triggers_to( tick-1, false, 1 );
            }


            ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
        }
    }

    updateMousePtr( ths );
    return true;
}


/* popup menu calls this */
void Seq24PerfInput::set_adding( bool a_adding, perfroll& ths )
{
    if ( a_adding )
    {
        ths.get_window()->set_cursor(  Gdk::Cursor( Gdk::PENCIL ));
        m_adding = true;
    }
    else
    {
        ths.get_window()->set_cursor( Gdk::Cursor( Gdk::LEFT_PTR ));
        m_adding = false;
    }
}

bool Seq24PerfInput::on_button_press_event(GdkEventButton* a_ev, perfroll& ths)
{
    ths.grab_focus( );


    if ( ths.m_mainperf->is_active( ths.m_drop_sequence ))
    {
        ths.m_mainperf->get_sequence( ths.m_drop_sequence )->unselect_triggers( );
        ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
    }

    ths.m_drop_x = (int) a_ev->x;
    ths.m_drop_y = (int) a_ev->y;

    ths.convert_xy( ths.m_drop_x, ths.m_drop_y, &ths.m_drop_tick, &ths.m_drop_sequence );

    /*      left mouse button     */
    if ( a_ev->button == 1 ){

        long tick = ths.m_drop_tick;

        /* add a new note if we didnt select anything */
        if (  m_adding ){

            m_adding_pressed = true;

            if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

                long seq_length = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_length( );

                bool state = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_trigger_state( tick );

                if ( state )
                {
                    ths.m_mainperf->push_trigger_undo();
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )->del_trigger( tick );
                }
                else
                {

                    // snap to length of sequence
                    tick = tick - (tick % seq_length);
                    //m_adding_pressed_state = true;

                    ths.m_mainperf->push_trigger_undo();
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )->add_trigger( tick, seq_length );
                    ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
                    ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
                    ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);

                    //m_drop_tick_last = (m_drop_tick + seq_length - 1);
                }
            }
        }
        else {

            if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

                ths.m_mainperf->push_trigger_undo();
                ths.m_mainperf->get_sequence( ths.m_drop_sequence )->select_trigger( tick );

                long start_tick = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_selected_trigger_start_tick();
                long end_tick = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_selected_trigger_end_tick();

                if ( tick >= start_tick &&
                        tick <= start_tick + (c_perfroll_size_box_click_w * c_perf_scale_x) &&
                        (ths.m_drop_y % c_names_y) <= c_perfroll_size_box_click_w + 1 )
                {
                    ths.m_growing = true;
                    ths.m_grow_direction = true;
                    ths.m_drop_tick_trigger_offset = ths.m_drop_tick -
                        ths.m_mainperf->get_sequence( ths.m_drop_sequence )->
                        get_selected_trigger_start_tick( );
                }
                else
                    if ( tick >= end_tick - (c_perfroll_size_box_click_w * c_perf_scale_x) &&
                            tick <= end_tick &&
                            (ths.m_drop_y % c_names_y) >= c_names_y - c_perfroll_size_box_click_w - 1 )
                    {
                        ths.m_growing = true;
                        ths.m_grow_direction = false;
                        ths.m_drop_tick_trigger_offset =
                            ths.m_drop_tick -
                            ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_selected_trigger_end_tick( );
                    }
                    else
                    {
                        ths.m_moving = true;
                        ths.m_drop_tick_trigger_offset = ths.m_drop_tick -
                            ths.m_mainperf->get_sequence( ths.m_drop_sequence )->
                            get_selected_trigger_start_tick( );

                    }

                ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
                ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
                ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
            }
        }
    }

    /*     right mouse button      */
    if ( a_ev->button == 3 ){
        set_adding( true, ths );
    }

    /* middle, split */
    if ( a_ev->button == 2 )
    {
        if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

            bool state = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_trigger_state( ths.m_drop_tick );

            if ( state )
            {
                ths.split_trigger(ths.m_drop_sequence, ths.m_drop_tick);
            }
        }
    }
    return true;
}

bool Seq24PerfInput::on_button_release_event(GdkEventButton* a_ev, perfroll& ths)
{
    if ( a_ev->button == 1 ){

        if ( m_adding ){
            m_adding_pressed = false;
        }
    }

    if ( a_ev->button == 3 ){
        m_adding_pressed = false;
        set_adding( false, ths );
    }

    ths.m_moving = false;
    ths.m_growing = false;
    m_adding_pressed = false;

    if ( ths.m_mainperf->is_active( ths.m_drop_sequence  )){

        ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
        ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y );
    }

    return true;
}

bool Seq24PerfInput::on_motion_notify_event(GdkEventMotion* a_ev, perfroll& ths)
{
    long tick;
    int x = (int) a_ev->x;

    if (  m_adding && m_adding_pressed ){

        ths.convert_x( x, &tick );

        if ( ths.m_mainperf->is_active( ths.m_drop_sequence )){

            long seq_length = ths.m_mainperf->get_sequence( ths.m_drop_sequence )->get_length( );
            tick = tick - (tick % seq_length);

            /*long min_tick = (tick < m_drop_tick) ? tick : m_drop_tick;*/
            long length = seq_length;

            ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                ->grow_trigger( ths.m_drop_tick, tick, length);
            ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
        }
    }
    else if ( ths.m_moving || ths.m_growing ){

        if ( ths.m_mainperf->is_active( ths.m_drop_sequence)){

            ths.convert_x( x, &tick );
            tick -= ths.m_drop_tick_trigger_offset;

            tick = tick - tick % ths.m_snap;

            if ( ths.m_moving )
            {
                ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                    ->move_selected_triggers_to( tick, true );
            }
            if ( ths.m_growing )
            {
                if ( ths.m_grow_direction )
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                        ->move_selected_triggers_to( tick, false, 0 );
                else
                    ths.m_mainperf->get_sequence( ths.m_drop_sequence )
                        ->move_selected_triggers_to( tick-1, false, 1 );
            }


            ths.draw_background_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_sequence_on( ths.m_pixmap, ths.m_drop_sequence );
            ths.draw_drawable_row( ths.m_window, ths.m_pixmap, ths.m_drop_y);
        }
    }

    return true;
}
