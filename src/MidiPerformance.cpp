/* holds all the seqs and handles their playback */

#include "MidiPerformance.hpp"
#include "MidiBus.hpp"
#include "MidiEvent.hpp"
#include <stdio.h>
#ifndef __WIN32__
#  include <time.h>
#endif
#include <sched.h>

//TODO restore this
//For keys
//#include <gtkmm/accelkey.h>

MidiPerformance::MidiPerformance()
{
    for (int i=0; i< c_max_sequence; i++) {

        m_seqs[i]             = NULL;
        m_seqs_active[i]      = false;

        m_was_active_main[i]  = false;
        m_was_active_edit[i]  = false;
        m_was_active_perf[i]  = false;
        m_was_active_names[i] = false;
    }

    m_mute_group_selected = 0;
    m_mode_group = true;
    m_mode_group_learn = false;
    m_running = false;
    m_looping = false;
    m_inputing = true;
    m_outputing = true;
    m_tick = 0;
    m_midiclockrunning = false;
    m_usemidiclock = false;
    m_midiclocktick = 0;
    m_midiclockpos = -1;

    thread_trigger_width_ms = c_thread_trigger_width_ms;

    m_left_tick = 0;
    m_right_tick = c_ppqn * 16;
    m_starting_tick = 0;

    MidiControl zero = {false,false,0,0,0,0};

    for ( int i=0; i<c_midi_controls; i++ ){

        m_midi_cc_toggle[i] = zero;
        m_midi_cc_on[i] = zero;
        m_midi_cc_off[i] = zero;
    }

    m_show_ui_sequence_key = true;

    //TODO restore this
//    set_key_event( GDK_1, 0 );
//    set_key_event( GDK_q, 1 );
//    set_key_event( GDK_a, 2 );
//    set_key_event( GDK_z, 3 );
//    set_key_event( GDK_2, 4 );
//    set_key_event( GDK_w, 5 );
//    set_key_event( GDK_s, 6 );
//    set_key_event( GDK_x, 7 );
//    set_key_event( GDK_3, 8 );
//    set_key_event( GDK_e, 9 );
//    set_key_event( GDK_d, 10 );
//    set_key_event( GDK_c, 11 );
//    set_key_event( GDK_4, 12 );
//    set_key_event( GDK_r, 13 );
//    set_key_event( GDK_f, 14 );
//    set_key_event( GDK_v, 15 );
//    set_key_event( GDK_5, 16 );
//    set_key_event( GDK_t, 17 );
//    set_key_event( GDK_g, 18 );
//    set_key_event( GDK_b, 19 );
//    set_key_event( GDK_6, 20 );
//    set_key_event( GDK_y, 21 );
//    set_key_event( GDK_h, 22 );
//    set_key_event( GDK_n, 23 );
//    set_key_event( GDK_7, 24 );
//    set_key_event( GDK_u, 25 );
//    set_key_event( GDK_j, 26 );
//    set_key_event( GDK_m, 27 );
//    set_key_event( GDK_8, 28 );
//    set_key_event( GDK_i, 29 );
//    set_key_event( GDK_k, 30 );
//    set_key_event( GDK_comma, 31 );

//    set_key_group( GDK_exclam,  0 );
//    set_key_group( GDK_quotedbl,  1  );
//    set_key_group( GDK_numbersign,  2  );
//    set_key_group( GDK_dollar,  3  );
//    set_key_group( GDK_percent,  4  );
//    set_key_group( GDK_ampersand,  5  );
//    set_key_group( GDK_parenleft,  7  );
//    set_key_group( GDK_slash,  6  );
//    set_key_group( GDK_semicolon,  31 );
//    set_key_group( GDK_A,  16 );
//    set_key_group( GDK_B,  28 );
//    set_key_group( GDK_C,  26 );
//    set_key_group( GDK_D,  18 );
//    set_key_group( GDK_E,  10 );
//    set_key_group( GDK_F,  19 );
//    set_key_group( GDK_G,  20 );
//    set_key_group( GDK_H,  21 );
//    set_key_group( GDK_I,  15 );
//    set_key_group( GDK_J,  22 );
//    set_key_group( GDK_K,  23 );
//    set_key_group( GDK_M,  30 );
//    set_key_group( GDK_N,  29 );
//    set_key_group( GDK_Q,  8  );
//    set_key_group( GDK_R,  11 );
//    set_key_group( GDK_S,  17 );
//    set_key_group( GDK_T,  12 );
//    set_key_group( GDK_U,  14 );
//    set_key_group( GDK_V,  27 );
//    set_key_group( GDK_W,  9  );
//    set_key_group( GDK_X,  25 );
//    set_key_group( GDK_Y,  13 );
//    set_key_group( GDK_Z,  24 );

//    m_key_bpm_up = GDK_apostrophe;
//    m_key_bpm_dn = GDK_semicolon;

//    m_key_replace = GDK_Control_L;
//    m_key_queue = GDK_Control_R;
//    m_key_snapshot_1 = GDK_Alt_L;
//    m_key_snapshot_2 = GDK_Alt_R;
//    m_key_keep_queue = GDK_backslash;

//    m_key_screenset_up = GDK_bracketright;
//    m_key_screenset_dn = GDK_bracketleft;
//    m_key_set_playing_screenset = GDK_Home;
//    m_key_group_on = GDK_igrave;
//    m_key_group_off = GDK_apostrophe;
//    m_key_group_learn = GDK_Insert;

//    m_key_start  = GDK_space;
//    m_key_stop   = GDK_Escape;
//    m_key_record_upper = GDK_R;
//    m_key_record_lower = GDK_r;

    m_offset                = 0;
    m_control_status        = 0;
    m_screen_set            = 0;

    m_jack_running          = false;
    m_jack_master           = false;

    m_out_thread_launched   = false;
    m_in_thread_launched    = false;

    m_playback_mode         = false;
}


void MidiPerformance::init()
{
    m_master_bus.init( );
}


void MidiPerformance::init_jack()
{

#ifdef JACK_SUPPORT

    if ( global_with_jack_transport  && !m_jack_running){

        m_jack_running = true;
        m_jack_master = true;

        printf ( "init_jack() m_jack_running[%d]\n", m_jack_running );

        do {

            /* become a new client of the JACK server */
#ifdef JACK_SESSION
        if (global_jack_session_uuid.isEmpty())
		m_jack_client = jack_client_open(PACKAGE, JackNullOption, NULL);
	    else
		m_jack_client = jack_client_open(PACKAGE, JackSessionID, NULL,
                        global_jack_session_uuid.toAscii().constData());
#else
	    m_jack_client = jack_client_open(PACKAGE, JackNullOption, NULL );
#endif

            if (m_jack_client == 0) {
                printf( "JACK server is not running.\n[JACK sync disabled]\n");
                m_jack_running = false;
                break;
            }

            jack_on_shutdown( m_jack_client, jack_shutdown,(void *) this );
            jack_set_sync_callback(m_jack_client, jack_sync_callback,
                    (void *) this );
            jack_set_process_callback(m_jack_client, jack_process_callback, NULL);
#ifdef JACK_SESSION
	    if (jack_set_session_callback)
		jack_set_session_callback(m_jack_client, jack_session_callback,
			(void *) this );
#endif

            /* true if we want to fail if there is already a master */
            bool cond = global_with_jack_master_cond;

            if ( global_with_jack_master &&
                    jack_set_timebase_callback(m_jack_client, cond,
                        jack_timebase_callback, this) == 0){

                printf("[JACK transport master]\n");
                m_jack_master = true;
            }
            else {
                printf("[JACK transport slave]\n");
                m_jack_master = false;

            }
            if (jack_activate(m_jack_client)) {
                printf("Cannot register as JACK client\n");
                m_jack_running = false;
                break;
            }
        } while (0);
    }

#endif
}


void MidiPerformance::deinit_jack()
{
#ifdef JACK_SUPPORT

    if ( m_jack_running){

        printf ( "deinit_jack() m_jack_running[%d]\n", m_jack_running );

        m_jack_running = false;
        m_jack_master = false;

        if ( jack_release_timebase(m_jack_client)){
            printf("Cannot release Timebase.\n");
        }

        if (jack_client_close(m_jack_client)) {
            printf("Cannot close JACK client.\n");
        }
    }

    if ( !m_jack_running ){
        printf( "[JACK sync disabled]\n");
    }

#endif
}


void MidiPerformance::clear_all()
{
    reset_sequences();

    for (int i=0; i< c_max_sequence; i++ ){

        if ( is_active(i) )
            delete_sequence( i );
    }

    string e( "" );

    for (int i=0; i<c_max_sets; i++ ){
        set_screen_set_notepad( i, &e );
    }
}


void MidiPerformance::set_mode_group_mute ()
{
    m_mode_group = true;
}

void MidiPerformance::unset_mode_group_mute ()
{
    m_mode_group = false;
}


void MidiPerformance::set_group_mute_state (int a_g_track, bool a_mute_state)
{
    if (a_g_track < 0)
        a_g_track = 0;
    if (a_g_track > c_seqs_in_set)
        a_g_track = c_seqs_in_set -1;
    m_mute_group[a_g_track + m_mute_group_selected * c_seqs_in_set] = a_mute_state;
}


bool MidiPerformance::get_group_mute_state (int a_g_track)
{
    if (a_g_track < 0)
        a_g_track = 0;
    if (a_g_track > c_seqs_in_set)
        a_g_track = c_seqs_in_set -1;
    return m_mute_group[a_g_track + m_mute_group_selected * c_seqs_in_set];
}


void MidiPerformance::select_group_mute (int a_g_mute)
{
    int j = (a_g_mute * c_seqs_in_set);
    int k = m_playing_screen * c_seqs_in_set;
    if (a_g_mute < 0)
        a_g_mute = 0;
    if (a_g_mute > c_seqs_in_set)
        a_g_mute = c_seqs_in_set -1;
    if (m_mode_group_learn)
        for (int i = 0; i < c_seqs_in_set; i++) {
            if (is_active(i + k)) {
                assert(m_seqs[i + k]);
                m_mute_group[i + j] = m_seqs[i + k]->get_playing();
            }
        }
    m_mute_group_selected = a_g_mute;
}


void MidiPerformance::set_mode_group_learn ()
{
    set_mode_group_mute();
    m_mode_group_learn = true;
    for (size_t x = 0; x < m_notify.size(); ++x)
        m_notify[x]->on_grouplearnchange( true );
}


void MidiPerformance::unset_mode_group_learn ()
{
    for (size_t x = 0; x < m_notify.size(); ++x)
        m_notify[x]->on_grouplearnchange( false );
    m_mode_group_learn = false;
}


void MidiPerformance::select_mute_group ( int a_group )
{
    int j = (a_group * c_seqs_in_set);
    int k = m_playing_screen * c_seqs_in_set;
    if (a_group < 0)
        a_group = 0;
    if (a_group > c_seqs_in_set)
        a_group = c_seqs_in_set -1;
    m_mute_group_selected = a_group;
    for (int i = 0; i < c_seqs_in_set; i++) {
        if ((m_mode_group_learn) && (is_active(i + k))) {
            assert(m_seqs[i + k]);
            m_mute_group[i + j] = m_seqs[i + k]->get_playing();
        }
        m_tracks_mute_state[i] = m_mute_group[i + m_mute_group_selected * c_seqs_in_set];
    }
}


void MidiPerformance::mute_group_tracks ()
{
    if (m_mode_group) {
        for (int i=0; i< c_seqs_in_set; i++) {
            for (int j=0; j < c_seqs_in_set; j++) {
                if ( is_active(i * c_seqs_in_set + j) ) {
                    if ((i == m_playing_screen) && (m_tracks_mute_state[j])) {
                        sequence_playing_on (i * c_seqs_in_set + j);
                    } else {
                        sequence_playing_off (i * c_seqs_in_set + j);
                    }
                }
            }

        }
    }
}


void MidiPerformance::select_and_mute_group (int a_g_group)
{
    select_mute_group(a_g_group);
    mute_group_tracks();
}


void MidiPerformance::mute_all_tracks()
{
    for (int i=0; i< c_max_sequence; i++ )
    {
        if ( is_active(i) )
            m_seqs[i]->set_song_mute( true );

    }
}


MidiPerformance::~MidiPerformance()
{
    m_inputing = false;
    m_outputing = false;
    m_running = false;

    m_condition_var.signal();

    if (m_out_thread_launched )
        pthread_join( m_out_thread, NULL );

    if (m_in_thread_launched )
        pthread_join( m_in_thread, NULL );

    for (int i=0; i< c_max_sequence; i++ ){
        if ( is_active(i) ){
            delete m_seqs[i];
        }
    }
}


void MidiPerformance::set_left_tick( long a_tick )
{
    m_left_tick = a_tick;
    m_starting_tick = a_tick;

    if ( m_left_tick >= m_right_tick )
        m_right_tick = m_left_tick + c_ppqn * 4;

}


long MidiPerformance::get_left_tick()
{
    return m_left_tick;
}


void MidiPerformance::set_starting_tick( long a_tick )
{
    m_starting_tick = a_tick;
}


long MidiPerformance::get_starting_tick()
{
    return m_starting_tick;
}


void MidiPerformance::set_right_tick( long a_tick )
{
    if ( a_tick >= c_ppqn * 4 ){

        m_right_tick = a_tick;

        if ( m_right_tick <= m_left_tick ){
            m_left_tick = m_right_tick - c_ppqn * 4;
            m_starting_tick = m_left_tick;
        }
    }
}


long MidiPerformance::get_right_tick()
{
    return m_right_tick;
}


void MidiPerformance::add_sequence(MidiSequence *a_seq, int a_perf )
{
    /* check for perferred */
    if ( a_perf < c_max_sequence &&
            is_active(a_perf) == false &&
            a_perf >= 0 ){

        m_seqs[a_perf] = a_seq;
        set_active(a_perf, true);
        //a_seq->set_tag( a_perf );

    } else {

        for (int i=a_perf; i< c_max_sequence; i++ ){

            if ( is_active(i) == false ){

                m_seqs[i] = a_seq;
                set_active(i,true);

                //a_seq->set_tag( i  );
                break;
            }
        }
    }
}


void MidiPerformance::set_active( int a_sequence, bool a_active )
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return;

    printf ("set_active %d\n", a_active );

    if ( m_seqs_active[ a_sequence ] == true && a_active == false )
    {
        set_was_active(a_sequence);
    }

    m_seqs_active[ a_sequence ] = a_active;
}


void MidiPerformance::set_was_active( int a_sequence )
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return;

    printf( "was_active true\n" );

    m_was_active_main[ a_sequence ] = true;
    m_was_active_edit[ a_sequence ] = true;
    m_was_active_perf[ a_sequence ] = true;
    m_was_active_names[ a_sequence ] = true;
}

bool MidiPerformance::is_active( int a_sequence )
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return false;

    return m_seqs_active[ a_sequence ];
}

bool MidiPerformance::is_dirty_main (int a_sequence)
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return false;

    if ( is_active(a_sequence) )
    {
        return m_seqs[a_sequence]->is_dirty_main();
    }

    bool was_active = m_was_active_main[ a_sequence ];
    m_was_active_main[ a_sequence ] = false;

    return was_active;
}

bool MidiPerformance::is_dirty_edit (int a_sequence)
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return false;

    if ( is_active(a_sequence) )
    {
        return m_seqs[a_sequence]->is_dirty_edit();
    }

    bool was_active = m_was_active_edit[ a_sequence ];
    m_was_active_edit[ a_sequence ] = false;

    return was_active;
}

bool MidiPerformance::is_dirty_perf (int a_sequence)
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return false;

    if ( is_active(a_sequence) )
    {
        return m_seqs[a_sequence]->is_dirty_perf();
    }

    bool was_active = m_was_active_perf[ a_sequence ];
    m_was_active_perf[ a_sequence ] = false;

    return was_active;
}

bool MidiPerformance::is_dirty_names (int a_sequence)
{
    if ( a_sequence < 0 || a_sequence >= c_max_sequence )
        return false;

    if ( is_active(a_sequence) )
    {
        return m_seqs[a_sequence]->is_dirty_names();
    }

    bool was_active = m_was_active_names[ a_sequence ];
    m_was_active_names[ a_sequence ] = false;

    return was_active;
}

MidiSequence *MidiPerformance::get_sequence(int MidiSequence )
{
    return m_seqs[MidiSequence];
}

MasterMidiBus* MidiPerformance::get_master_midi_bus( )
{
    return &m_master_bus;
}

void MidiPerformance::set_running( bool a_running )
{
    m_running = a_running;
}

bool MidiPerformance::is_running()
{
    return m_running;
}

void MidiPerformance::set_bpm(int a_bpm)
{
    if ( a_bpm < 20 )  a_bpm = 20;
    if ( a_bpm > 500 ) a_bpm = 500;

    if ( ! (m_jack_running && m_running )){
        m_master_bus.set_bpm( a_bpm );
    }
}

int  MidiPerformance::get_bpm( )
{
    return  m_master_bus.get_bpm( );
}


void MidiPerformance::delete_sequence( int a_num )
{
    set_active(a_num, false);

    if ( m_seqs[a_num] != NULL &&
            !m_seqs[a_num]->get_editing() ){

        m_seqs[a_num]->set_playing( false );
        delete m_seqs[a_num];
    }
}


bool MidiPerformance::is_sequence_in_edit( int a_num )
{
    return ( m_seqs[a_num] != NULL &&
            m_seqs[a_num]->get_editing());

}

void MidiPerformance::new_sequence( int a_sequence )
{
    m_seqs[ a_sequence ] = new MidiSequence();
    m_seqs[ a_sequence ]->set_master_midi_bus( &m_master_bus );
    set_active(a_sequence, true);

}

MidiControl * MidiPerformance::get_midi_control_toggle( unsigned int a_seq )
{
    if ( a_seq >= (unsigned int) c_midi_controls )
        return NULL;
    return &m_midi_cc_toggle[a_seq];

}


MidiControl * MidiPerformance::get_midi_control_on( unsigned int a_seq )
{
    if ( a_seq >= (unsigned int) c_midi_controls )
        return NULL;
    return &m_midi_cc_on[a_seq];
}


MidiControl* MidiPerformance::get_midi_control_off( unsigned int a_seq )
{
    if ( a_seq >= (unsigned int) c_midi_controls )
        return NULL;
    return &m_midi_cc_off[a_seq];
}


void MidiPerformance::print()
{
    //   for( int i=0; i<m_numSeq; i++ ){

    //printf("Sequence %d\n", i);
    //m_seqs[i]->print();
    // }

    //  m_master_bus.print();
}


void MidiPerformance::set_screen_set_notepad( int a_screen_set, string *a_notepad )
{
    if ( a_screen_set < c_max_sets )
        m_screen_set_notepad[a_screen_set] = *a_notepad;
}


string * MidiPerformance::get_screen_set_notepad( int a_screen_set )
{
    return &m_screen_set_notepad[a_screen_set];
}


void MidiPerformance::set_screenset( int a_ss )
{
    m_screen_set = a_ss;

    if ( m_screen_set < 0 )
        m_screen_set = c_max_sets - 1;

    if ( m_screen_set >= c_max_sets )
        m_screen_set = 0;
}


int MidiPerformance::get_screenset()
{
    return m_screen_set;
}


void MidiPerformance::  set_playing_screenset ()
{
    for (int j, i = 0; i < c_seqs_in_set; i++) {
        j = i + m_playing_screen * c_seqs_in_set;
        if ( is_active(j) ){
            assert( m_seqs[j] );
            m_tracks_mute_state[i] = m_seqs[j]->get_playing();
        }
    }
    m_playing_screen = m_screen_set;
    mute_group_tracks();
}


int MidiPerformance::get_playing_screenset ()
{
    return m_playing_screen;
}


void MidiPerformance::set_offset( int a_offset )
{
    m_offset = a_offset  * c_mainwnd_rows * c_mainwnd_cols;
}


void MidiPerformance::play( long a_tick )
{

    /* just run down the list of sequences and have them dump */

        printf( "play [%d]\n", a_tick );

    m_tick = a_tick;	

    /* for all seqs in the array */
    for (int i=0; i< c_max_sequence; i++ ){

        if ( is_active(i) ){

            assert( m_seqs[i] );

//            printf( "dump seq - [%s]\n", m_seqs[i]->get_name() );

            if ( m_seqs[i]->get_queued() &&

                    m_seqs[i]->get_queued_tick() <= a_tick ){

                m_seqs[i]->play( m_seqs[i]->get_queued_tick() - 1, m_playback_mode );

                m_seqs[i]->toggle_playing();

                printf( "toggle playing - [%s]\n", m_seqs[i]->get_name() );
            }

            m_seqs[i]->play( a_tick, m_playback_mode );
        }
    }

    /* flush the bus */
    m_master_bus.flush();
}


void MidiPerformance::set_orig_ticks( long a_tick  )
{
    for (int i=0; i< c_max_sequence; i++ ){

        if ( is_active(i) == true ){

            assert( m_seqs[i] );

            m_seqs[i]->set_orig_tick( a_tick );
        }
    }
}


void MidiPerformance::clear_sequence_triggers( int a_seq  )
{
    if ( is_active(a_seq) == true ){

        assert( m_seqs[a_seq] );

        m_seqs[a_seq]->clear_triggers( );
    }
}


void MidiPerformance::move_triggers( bool a_direction )
{
    if ( m_left_tick < m_right_tick ){

        long distance = m_right_tick - m_left_tick;

        for (int i=0; i< c_max_sequence; i++ ){

            if ( is_active(i) == true ){

                assert( m_seqs[i] );

                m_seqs[i]->move_triggers( m_left_tick, distance, a_direction );
            }
        }
    }
}


void MidiPerformance::push_trigger_undo()
{
    for (int i=0; i< c_max_sequence; i++ ){

        if ( is_active(i) == true ){
            assert( m_seqs[i] );
            m_seqs[i]->push_trigger_undo( );
        }
    }
}


void MidiPerformance::pop_trigger_undo()
{
    for (int i=0; i< c_max_sequence; i++ ){

        if ( is_active(i) == true ){
            assert( m_seqs[i] );
            m_seqs[i]->pop_trigger_undo( );
        }
    }
}


/* copies between L and R -> R */
void MidiPerformance::copy_triggers( )
{
    if ( m_left_tick < m_right_tick ){

        long distance = m_right_tick - m_left_tick;

        for (int i=0; i< c_max_sequence; i++ ){

            if ( is_active(i) == true ){
                assert( m_seqs[i] );
                m_seqs[i]->copy_triggers( m_left_tick, distance );
            }
        }
    }
}


void MidiPerformance::start_jack(  )
{
    //printf( "perform::start_jack()\n" );
#ifdef JACK_SUPPORT
    if ( m_jack_running)
        jack_transport_start (m_jack_client );
#endif
}


void MidiPerformance::stop_jack(  )
{
    //printf( "perform::stop_jack()\n" );
#ifdef JACK_SUPPORT
    if( m_jack_running )
        jack_transport_stop (m_jack_client);
#endif
}


void MidiPerformance::position_jack( bool a_state )
{

    //printf( "perform::position_jack()\n" );

#ifdef JACK_SUPPORT

    if ( m_jack_running ){
        jack_transport_locate( m_jack_client, 0 );
    }
    return;


    jack_nframes_t rate = jack_get_sample_rate( m_jack_client ) ;

    long current_tick = 0;

    if ( a_state ){
        current_tick = m_left_tick;
    }

    jack_position_t pos;

    pos.valid = JackPositionBBT;
    pos.beats_per_bar = 4;
    pos.beat_type = 4;
    pos.ticks_per_beat = c_ppqn * 10;
    pos.beats_per_minute =  m_master_bus.get_bpm();

    /* Compute BBT info from frame number.  This is relatively
     * simple here, but would become complex if we supported tempo
     * or time signature changes at specific locations in the
     * transport timeline. */

    current_tick *= 10;

    pos.bar = (int32_t) (current_tick / (long) pos.ticks_per_beat
            / pos.beats_per_bar);
    pos.beat = (int32_t) ((current_tick / (long) pos.ticks_per_beat) % 4);
    pos.tick = (int32_t) (current_tick % (c_ppqn * 10));

    pos.bar_start_tick = pos.bar * pos.beats_per_bar * pos.ticks_per_beat;
    pos.frame_rate = rate;
    pos.frame = (jack_nframes_t) ( (current_tick * rate * 60.0)
            / (pos.ticks_per_beat * pos.beats_per_minute) );

    /*
       ticks * 10 = jack ticks;
       jack ticks / ticks per beat = num beats;
       num beats / beats per minute = num minutes
       num minutes * 60 = num seconds
       num secords * frame_rate  = frame */

    pos.bar++;
    pos.beat++;

    //printf( "position bbb[%d:%d:%4d]\n", pos.bar, pos.beat, pos.tick );

    jack_transport_reposition( m_jack_client, &pos );

#endif
}


void MidiPerformance::start(bool a_state)
{
    if (m_jack_running) {
        return;
    }

    inner_start(a_state);
}


void MidiPerformance::stop()
{
    if (m_jack_running) {
        return;
    }

    inner_stop();
}


void MidiPerformance::inner_start(bool a_state)
{
    m_condition_var.lock();

    if (!is_running()) {

        set_playback_mode( a_state );

        if (a_state)
            off_sequences();

        set_running(true);
        m_condition_var.signal();
    }

    m_condition_var.unlock();
}


void MidiPerformance::inner_stop()
{
    set_running(false);
    //off_sequences();
    reset_sequences();
    m_usemidiclock = false;
}


void MidiPerformance::off_sequences()
{
    for (int i = 0; i < c_max_sequence; i++) {

        if (is_active(i)) {
            assert(m_seqs[i]);
            m_seqs[i]->set_playing(false);
        }
    }
}


void MidiPerformance::all_notes_off()
{
    for (int i=0; i< c_max_sequence; i++) {

        if (is_active(i)) {
            assert(m_seqs[i]);
            m_seqs[i]->off_playing_notes();
        }
    }
    /* flush the bus */
    m_master_bus.flush();
}


void MidiPerformance::reset_sequences()
{
    for (int i=0; i< c_max_sequence; i++) {

        if (is_active(i)) {
            assert( m_seqs[i] );

            bool state = m_seqs[i]->get_playing();

            m_seqs[i]->off_playing_notes();
            m_seqs[i]->set_playing(false);
            m_seqs[i]->zero_markers();

            if (!m_playback_mode)
                m_seqs[i]->set_playing(state);
        }
    }
    /* flush the bus */
    m_master_bus.flush();
}


void MidiPerformance::launch_output_thread()
{
    int err;

    err = pthread_create(&m_out_thread, NULL, output_thread_func, this);
    if (err != 0) {
        /*TODO: error handling*/
    }
    else
        m_out_thread_launched= true;
}


void MidiPerformance::set_playback_mode( bool a_playback_mode )
{
    m_playback_mode = a_playback_mode;
    printf("\nPlayback mode - %i\n", a_playback_mode);
}

bool MidiPerformance::get_playback_mode()
{
    return m_playback_mode;
}

void MidiPerformance::set_song_recording(bool new_state){
    m_song_recording = new_state;
}

bool MidiPerformance::get_song_recording(){
    return m_song_recording;
}

void MidiPerformance::launch_input_thread()
{
    int err;

    err = pthread_create(&m_in_thread, NULL, input_thread_func, this);
    if (err != 0) {
        /*TODO: error handling*/
    }
    else
    m_in_thread_launched = true;
}


long MidiPerformance::get_max_trigger()
{
    long ret = 0, t;

    for (int i=0; i< c_max_sequence; i++ ){

        if ( is_active(i) == true ){
            assert( m_seqs[i] );

            t = m_seqs[i]->get_max_trigger( );
            if ( t > ret )
                ret = t;
        }
    }

    return ret;
}


void* output_thread_func(void *a_pef )
{
    /* set our performance */
    MidiPerformance *p = (MidiPerformance *) a_pef;
    assert(p);

    struct sched_param schp;
    /*
     * set the process to realtime privs
     */

    if ( global_priority ){

        memset(&schp, 0, sizeof(sched_param));
        schp.sched_priority = 1;

#ifndef __WIN32__
        // Not in MinGW RCB
        if (sched_setscheduler(0, SCHED_FIFO, &schp) != 0) 	{
            printf("output_thread_func: couldnt sched_setscheduler"
                    " (FIFO), you need to be root.\n");
            pthread_exit(0);
        }
#endif
    }

#ifdef __WIN32__
    timeBeginPeriod(1);
#endif
    p->output_func();
#ifdef __WIN32__
    timeEndPeriod(1);
#endif

    return 0;
}

#ifdef JACK_SUPPORT

int jack_process_callback(jack_nframes_t nframes, void* arg)
{return 0;}

int jack_sync_callback(jack_transport_state_t state,
        jack_position_t *pos, void *arg)
{
    MidiPerformance *p = (MidiPerformance *) arg;

    p->m_jack_frame_current = jack_get_current_transport_frame(p->m_jack_client);

    p->m_jack_tick =
        p->m_jack_frame_current *
        p->m_jack_pos.ticks_per_beat *
        p->m_jack_pos.beats_per_minute / (p->m_jack_pos.frame_rate * 60.0);

    p->m_jack_frame_last = p->m_jack_frame_current;

    p->m_jack_transport_state_last =
        p->m_jack_transport_state =
        state;


    switch (state) {

        case JackTransportStopped:
            //printf( "[JackTransportStopped]\n" );
            break;

        case JackTransportRolling:
            //printf( "[JackTransportRolling]\n" );
            break;

        case JackTransportStarting:
            //printf( "[JackTransportStarting]\n" );
            p->inner_start( global_jack_start_mode );
            break;

        case JackTransportLooping:
            //printf( "[JackTransportLooping]" );
            break;

        default:
            break;
    }

    //printf( "starting frame[%d] tick[%8.2f]\n", p->m_jack_frame_current, p->m_jack_tick );

    print_jack_pos( pos );
    return 1;
}

#ifdef JACK_SESSION

bool MidiPerformance::jack_session_event()
{
    QString fname( m_jsession_ev->session_dir );
    fname += "file.mid";

    QString cmd( "kepler34 \"${SESSION_DIR}file.mid\" --jack_session_uuid " );
    cmd += m_jsession_ev->client_uuid;

    MidiFile f(fname);
    f.write(this);

    m_jsession_ev->command_line = strdup( cmd.toAscii().constData() );

    jack_session_reply( m_jack_client, m_jsession_ev );

    if( m_jsession_ev->type == JackSessionSaveAndQuit )
        QCoreApplication::exit(0);

    jack_session_event_free (m_jsession_ev);

    return false;
}

void jack_session_callback(jack_session_event_t *event, void *arg )
{
    MidiPerformance *p = (MidiPerformance *) arg;
    p->m_jsession_ev = event;
    //TODO restore
//    Glib::signal_idle().connect( sigc::mem_fun( *p, &perform::jack_session_event) );
}

#endif
#endif


void MidiPerformance::output_func()
{
    while (m_outputing) {

        //printf ("waiting for signal\n");

        m_condition_var.lock();

        while (!m_running) {

            m_condition_var.wait();

            /* if stopping, then kill thread */
            if (!m_outputing)
                break;
        }

        m_condition_var.unlock();

        printf( "signaled [%d]\n", m_playback_mode );

#ifndef __WIN32__
        /* begning time */
        struct timespec last;
        /* current time */
        struct timespec current;

        struct timespec stats_loop_start;
        struct timespec stats_loop_finish;


        /* difference between last and current */
        struct timespec delta;
#else
        /* begning time */
        long last;
        /* current time */
        long current;

        long stats_loop_start = 0;
        long stats_loop_finish = 0;


        /* difference between last and current */
        long delta;
#endif

        /* tick and tick fraction */
        double current_tick   = 0.0;
        double total_tick   = 0.0;
        long clock_tick = 0;
        long delta_tick_frac = 0;

        long stats_total_tick = 0;

        long stats_loop_index = 0;
        long stats_min = 0x7FFFFFFF;
        long stats_max = 0;
        long stats_avg = 0;
        long stats_last_clock_us = 0;
        long stats_clock_width_us = 0;

        long stats_all[100];
        long stats_clock[100];

        bool jack_stopped = false;
        bool dumping = false;

        bool init_clock = true;

#ifdef JACK_SUPPORT
        double jack_ticks_converted = 0.0;
        double jack_ticks_converted_last = 0.0;
        double jack_ticks_delta = 0.0;
#endif
        for( int i=0; i<100; i++ ){
            stats_all[i] = 0;
            stats_clock[i] = 0;
        }

        /* if we are in the performance view, we care
           about starting from the offset */
        if ( m_playback_mode && !m_jack_running){
//          if (!m_jack_running){

            current_tick = m_starting_tick;
            clock_tick = m_starting_tick;
            set_orig_ticks( m_starting_tick );
        }


        int ppqn = m_master_bus.get_ppqn();
#ifndef __WIN32__
        /* get start time position */
        clock_gettime(CLOCK_REALTIME, &last);

        if ( global_stats )
            stats_last_clock_us= (last.tv_sec * 1000000) + (last.tv_nsec / 1000);
#else
        /* get start time position */
        /* timeGetTime() returns a "DWORD" type (= unsigned long)*/
        last = timeGetTime();

        if ( global_stats )
            stats_last_clock_us= last * 1000;
#endif

        while( m_running ){

            /************************************

              Get delta time ( current - last )
              Get delta ticks from time
              Add to current_ticks
              Compute prebuffer ticks
              play from current tick to prebuffer

             **************************************/

            if ( global_stats ){
#ifndef __WIN32__
                clock_gettime(CLOCK_REALTIME, &stats_loop_start);
#else
                stats_loop_start = timeGetTime();
#endif
            }


            /* delta time */
#ifndef __WIN32__
            clock_gettime(CLOCK_REALTIME, &current);
            delta.tv_sec  =  (current.tv_sec  - last.tv_sec  );
            delta.tv_nsec =  (current.tv_nsec - last.tv_nsec );
            long delta_us = (delta.tv_sec * 1000000) + (delta.tv_nsec / 1000);
#else
            current = timeGetTime();
            //printf( "current [0x%x]\n", current );
            delta = current - last;
            long delta_us = delta * 1000;
            //printf( "  delta [0x%x]\n", delta );
#endif

            /* delta time to ticks */
            /* bpm */
            int bpm  = m_master_bus.get_bpm();

            /* get delta ticks, delta_ticks_f is in 1000th of a tick */
            long long delta_tick_num = bpm * ppqn * delta_us + delta_tick_frac;
            long long delta_tick_denom = 60000000;
            long delta_tick = (long)(delta_tick_num / delta_tick_denom);
            delta_tick_frac = (long)(delta_tick_num % delta_tick_denom);

            if (m_usemidiclock) {
                delta_tick = m_midiclocktick;
                m_midiclocktick = 0;
            }
            if (0 <= m_midiclockpos) {
                delta_tick = 0;
                clock_tick     = m_midiclockpos;
                current_tick   = m_midiclockpos;
                total_tick     = m_midiclockpos;
                m_midiclockpos = -1;
                //init_clock = true;
            }

            //printf ( "    delta_tick[%lf]\n", delta_tick  );
#ifdef JACK_SUPPORT

            // no init until we get a good lock

            if ( m_jack_running ){

                init_clock = false;

                m_jack_transport_state = jack_transport_query( m_jack_client, &m_jack_pos );
                m_jack_frame_current =  jack_get_current_transport_frame( m_jack_client );

                if ( m_jack_transport_state_last  ==  JackTransportStarting &&
                        m_jack_transport_state       == JackTransportRolling ){

                    m_jack_frame_last = m_jack_frame_current;


                    //printf ("[Start Playback]\n" );
                    dumping = true;
                    m_jack_tick =
                        m_jack_pos.frame *
                        m_jack_pos.ticks_per_beat *
                        m_jack_pos.beats_per_minute / (m_jack_pos.frame_rate * 60.0);


                    /* convert ticks */
                    jack_ticks_converted =
                        m_jack_tick * ((double) c_ppqn /
                                (m_jack_pos.ticks_per_beat *
                                 m_jack_pos.beat_type / 4.0  ));

                    set_orig_ticks( (long) jack_ticks_converted );
                    current_tick = clock_tick = total_tick = jack_ticks_converted_last = jack_ticks_converted;
                    init_clock = true;

                    if ( m_looping && m_playback_mode ){
//                    if (m_playback_mode){

                        //printf( "left[%lf] right[%lf]\n", (double) get_left_tick(), (double) get_right_tick() );

                        if ( current_tick >= get_right_tick() ){

                            while ( current_tick >= get_right_tick() ){

                                double size = get_right_tick() - get_left_tick();
                                current_tick = current_tick - size;

                                //printf( "> current_tick[%lf]\n", current_tick );
                            }
                            reset_sequences();
                            set_orig_ticks( (long)current_tick );
                        }
                    }
                }

                if ( m_jack_transport_state_last  ==  JackTransportRolling &&
                        m_jack_transport_state  == JackTransportStopped ){

                    m_jack_transport_state_last = JackTransportStopped;
                    //printf ("[Stop Playback]\n" );
                    jack_stopped = true;
                }

                //-----  Jack transport is Rolling Now ---------

                /* transport is in a sane state if dumping == true */
                if ( dumping )
                {
                    m_jack_frame_current =  jack_get_current_transport_frame( m_jack_client );

                    //printf( " frame[%7d]", m_jack_pos.frame );
                    //printf( " current_transport_frame[%7d]", m_jack_frame_current );

                    // if we are moving ahead
                    if ( (m_jack_frame_current > m_jack_frame_last)){


                        m_jack_tick +=
                            (m_jack_frame_current - m_jack_frame_last)  *
                            m_jack_pos.ticks_per_beat *
                            m_jack_pos.beats_per_minute / (m_jack_pos.frame_rate * 60.0);


                        //printf ( "m_jack_tick += (m_jack_frame_current[%lf] - m_jack_frame_last[%lf]) *\n",
                        //        (double) m_jack_frame_current, (double) m_jack_frame_last );
                        //printf(  "m_jack_pos.ticks_per_beat[%lf] * m_jack_pos.beats_per_minute[%lf] / \n(m_jack_pos.frame_rate[%lf] * 60.0\n", (double) m_jack_pos.ticks_per_beat, (double) m_jack_pos.beats_per_minute, (double) m_jack_pos.frame_rate);


                        m_jack_frame_last = m_jack_frame_current;
                    }

                    /* convert ticks */
                    jack_ticks_converted =
                        m_jack_tick * ((double) c_ppqn /
                                (m_jack_pos.ticks_per_beat * m_jack_pos.beat_type / 4.0  ));

                    //printf ( "jack_ticks_conv[%lf] = \n",  jack_ticks_converted );
                    //printf ( "    m_jack_tick[%lf] * ((double) c_ppqn[%lf] / \n", m_jack_tick, (double) c_ppqn );
                    //printf ( "   (m_jack_pos.ticks_per_beat[%lf] * m_jack_pos.beat_type[%lf] / 4.0  )\n",
                    //        m_jack_pos.ticks_per_beat, m_jack_pos.beat_type );




                    jack_ticks_delta = jack_ticks_converted - jack_ticks_converted_last;

                    clock_tick     += jack_ticks_delta;
                    current_tick   += jack_ticks_delta;
                    total_tick     += jack_ticks_delta;

                    m_jack_transport_state_last = m_jack_transport_state;
                    jack_ticks_converted_last = jack_ticks_converted;

                    /* printf( "current_tick[%lf] delta[%lf]\n", current_tick, jack_ticks_delta ); */



                    long pbeat = (long) ((long) m_jack_tick % (long) (m_jack_pos.ticks_per_beat *  m_jack_pos.beats_per_bar ));
                    pbeat = pbeat / (long) m_jack_pos.ticks_per_beat;

                    //long ptick = (long) m_jack_tick % (long) m_jack_pos.ticks_per_beat;
                    //long pbar  = (long) ((long) m_jack_tick / (m_jack_pos.ticks_per_beat *  m_jack_pos.beats_per_bar ));


                    //printf( " bbb [%2d:%2d:%4d]", pbar+1, pbeat+1, ptick );
                    //printf( " bbb [%2d:%2d:%4d]", m_jack_pos.bar, m_jack_pos.beat, m_jack_pos.tick );

                    /*double jack_tick = (m_jack_pos.bar-1) * (m_jack_pos.ticks_per_beat *  m_jack_pos.beats_per_bar ) +
                      (m_jack_pos.beat-1) * m_jack_pos.ticks_per_beat + m_jack_pos.tick;*/

                    //printf( " jtick[%8.3f]", m_jack_tick );
                    //printf( " mtick[%8.3f]", jack_tick );

                    //printf( " delta[%8.3f]", m_jack_tick - jack_tick );

                    //printf( "\n");

                } /* end if dumping / sane state */

            } /* if jack running */
            else
            {
#endif
                /* default if jack is not compiled in, or not running */
                /* add delta to current ticks */
                clock_tick     += delta_tick;
                current_tick   += delta_tick;
                total_tick     += delta_tick;
                dumping = true;

#ifdef JACK_SUPPORT
            }
#endif

            /* init_clock will be true when we run for the first time, or
             * as soon as jack gets a good lock on playback */

            if (init_clock) {
                m_master_bus.init_clock( clock_tick );
                init_clock = false;
            }

            if (dumping) {
                if ( m_looping && m_playback_mode )
//                if ( m_looping)
                {
                    if ( current_tick >= get_right_tick() )
                    {
                        double leftover_tick = current_tick - (get_right_tick());

                        play( get_right_tick() - 1 );
                        reset_sequences();

                        set_orig_ticks( get_left_tick() );
                        current_tick = (double) get_left_tick() + leftover_tick;
                    }
                }

                /* play */
                play( (long) current_tick );
                printf( "play[%d]\n", current_tick );

                /* midi clock */
                m_master_bus.clock( clock_tick );


                if ( global_stats ){	

                    while ( stats_total_tick <= total_tick ){

                        /* was there a tick ? */
                        if ( stats_total_tick % (c_ppqn / 24) == 0 ){

#ifndef __WIN32__
                            long current_us = (current.tv_sec * 1000000) + (current.tv_nsec / 1000);
#else
                            long current_us = current * 1000;
#endif
                            stats_clock_width_us = current_us - stats_last_clock_us;
                            stats_last_clock_us = current_us;

                            int index = stats_clock_width_us / 300;
                            if ( index >= 100 ) index = 99;
                            stats_clock[index]++;

                        }
                        stats_total_tick++;
                    }
                }
            }

            /***********************************

              Figure out how much time
              we need to sleep, and do it

             ************************************/

            /* set last */
            last = current;

#ifndef __WIN32__
            clock_gettime(CLOCK_REALTIME, &current);
            delta.tv_sec  =  (current.tv_sec  - last.tv_sec  );
            delta.tv_nsec =  (current.tv_nsec - last.tv_nsec );
            long elapsed_us = (delta.tv_sec * 1000000) + (delta.tv_nsec / 1000);
            //printf( "elapsed_us[%ld]\n", elapsed_us );
#else
            current = timeGetTime();
            delta = current - last;
            long elapsed_us = delta * 1000;
            //printf( "        elapsed_us[%ld]\n", elapsed_us );
#endif

            /* now, we want to trigger every c_thread_trigger_width_ms,
               and it took us delta_us to play() */

            delta_us = (c_thread_trigger_width_ms * 1000) - elapsed_us;
            //printf( "sleeping_us[%ld]\n", delta_us );


            /* check midi clock adjustment */

            double next_total_tick = (total_tick + (c_ppqn / 24.0));
            double next_clock_delta   = (next_total_tick - total_tick - 1);


            double next_clock_delta_us =  (( next_clock_delta ) * 60000000.0f / c_ppqn  / bpm );

            if ( next_clock_delta_us < (c_thread_trigger_width_ms * 1000.0f * 2.0f) ){
                delta_us = (long)next_clock_delta_us;
            }

#ifndef __WIN32__
            if ( delta_us > 0.0 ){

                delta.tv_sec =  (delta_us / 1000000);
                delta.tv_nsec = (delta_us % 1000000) * 1000;

                //printf("sleeping() ");
                nanosleep( &delta, NULL );

            }
#else
            if ( delta_us > 0 ){

                delta =  (delta_us / 1000);

                //printf("           sleeping() [0x%x]\n", delta);
                Sleep(delta);

            }
#endif

            else {

                if ( global_stats )
                    printf ("underrun\n" );
            }

            if ( global_stats ){
#ifndef __WIN32__	
                clock_gettime(CLOCK_REALTIME, &stats_loop_finish);
#else
                stats_loop_finish = timeGetTime();
#endif
            }

            if ( global_stats ){

#ifndef __WIN32__
                delta.tv_sec  =  (stats_loop_finish.tv_sec  - stats_loop_start.tv_sec  );
                delta.tv_nsec =  (stats_loop_finish.tv_nsec - stats_loop_start.tv_nsec );
                long delta_us = (delta.tv_sec * 1000000) + (delta.tv_nsec / 1000);
#else
                delta = stats_loop_finish - stats_loop_start;
                long delta_us = delta * 1000;
#endif

                int index = delta_us / 100;
                if ( index >= 100  ) index = 99;

                stats_all[index]++;

                if ( delta_us > stats_max )
                    stats_max = delta_us;
                if ( delta_us < stats_min )
                    stats_min = delta_us;

                stats_avg += delta_us;
                stats_loop_index++;	

                if ( stats_loop_index > 200 ){

                    stats_loop_index = 0;
                    stats_avg /= 200;

                    printf("stats_avg[%ld]us stats_min[%ld]us"
                            " stats_max[%ld]us\n", stats_avg,
                            stats_min, stats_max);

                    stats_min = 0x7FFFFFFF;
                    stats_max = 0;
                    stats_avg = 0;
                }
            }

            if (jack_stopped)
                inner_stop();
        }


        if (global_stats){

            printf("\n\n-- trigger width --\n");
            for (int i=0; i<100; i++ ){
                printf( "[%3d][%8ld]\n", i * 100, stats_all[i] );
            }
            printf("\n\n-- clock width --\n" );
            int bpm  = m_master_bus.get_bpm();

            printf("optimal: [%d]us\n", ((c_ppqn / 24)* 60000000 / c_ppqn / bpm));

            for ( int i=0; i<100; i++ ){
                printf( "[%3d][%8ld]\n", i * 300, stats_clock[i] );
            }
        }

        m_tick = 0;
        m_master_bus.flush();
        m_master_bus.stop();
    }

    pthread_exit(0);
}


void* input_thread_func(void *a_pef )
{

    /* set our performance */
    MidiPerformance *p = (MidiPerformance *) a_pef;
    assert(p);


    struct sched_param schp;
    /*
     * set the process to realtime privs
     */

    if ( global_priority ){

        memset(&schp, 0, sizeof(sched_param));
        schp.sched_priority = 1;

#ifndef __WIN32__
        // MinGW RCB
        if (sched_setscheduler(0, SCHED_FIFO, &schp) != 0) 	{

            printf("input_thread_func: couldnt sched_setscheduler"
                    " (FIFO), you need to be root.\n");
            pthread_exit(0);
        }
#endif
    }
#ifdef __WIN32__
    timeBeginPeriod(1);
#endif

    p->input_func();

#ifdef __WIN32__
    timeEndPeriod(1);
#endif

    return 0;
}


void MidiPerformance::handle_midi_control( int a_control, bool a_state )
{

    switch (a_control) {

        case c_midi_control_bpm_up:
            //printf ( "bpm up\n" );
            set_bpm( get_bpm() + 1 );
            break;

        case c_midi_control_bpm_dn:
            //printf ( "bpm dn\n" );
            set_bpm( get_bpm() - 1 );
            break;

        case c_midi_control_ss_up:
            //printf ( "ss up\n" );
            set_screenset( get_screenset() + 1 );
            break;

        case c_midi_control_ss_dn:
            //printf ( "ss dn\n" );
            set_screenset( get_screenset() - 1 );
            break;

        case c_midi_control_mod_replace:
            //printf ( "replace\n" );
            if ( a_state )
                set_sequence_control_status( c_status_replace );
            else
                unset_sequence_control_status( c_status_replace );
            break;

        case c_midi_control_mod_snapshot:
            //printf ( "snapshot\n" );
            if ( a_state )
                set_sequence_control_status( c_status_snapshot );
            else
                unset_sequence_control_status( c_status_snapshot );
            break;

        case c_midi_control_mod_queue:
            //printf ( "queue\n" );
            if ( a_state )
                set_sequence_control_status( c_status_queue );
            else
                unset_sequence_control_status( c_status_queue );

            //andy cases
        case c_midi_control_mod_gmute:
            printf ( "gmute\n" );
            if (a_state)
                set_mode_group_mute();
            else
                unset_mode_group_mute();
            break;

        case c_midi_control_mod_glearn:
            //printf ( "glearn\n" );
            if (a_state)
                set_mode_group_learn();
            else
                unset_mode_group_learn();
            break;

        case c_midi_control_play_ss:
            //printf ( "play_ss\n" );
            set_playing_screenset();
            break;

        default:
            if ((a_control >= c_seqs_in_set) && (a_control < c_midi_track_ctrl)) {
                //printf ( "group mute\n" );
                select_and_mute_group(a_control - c_seqs_in_set);
            }
            break;
    }
}


void MidiPerformance::input_func()
{
    MidiEvent ev;

    while (m_inputing) {

        if ( m_master_bus.poll_for_midi() > 0 ){

            do {

                if (m_master_bus.get_midi_event(&ev) ){

                    // Obey MidiTimeClock:
                    if (ev.get_status() == EVENT_MIDI_START)
                    {
                        stop();
                        start(false);
                        m_midiclockrunning = true;
                        m_usemidiclock = true;
                        m_midiclocktick = 0;
                        m_midiclockpos = 0;
                    }
                    // midi continue: start from current pos.
                    else if (ev.get_status() == EVENT_MIDI_CONTINUE)
                    {
                        m_midiclockrunning = true;
                        start(false);
                        //m_usemidiclock = true;
                    }
                    else if (ev.get_status() == EVENT_MIDI_STOP)
                    {
                        // do nothing, just let the system pause
                        // since we're not getting ticks after the stop, the song wont advance
                        // when start is recieved, we'll reset the position, or
                        // when continue is recieved, we wont
                        m_midiclockrunning = false;
                        all_notes_off();
                    }
                    else if (ev.get_status() == EVENT_MIDI_CLOCK)
                    {
                        if (m_midiclockrunning)
                            m_midiclocktick += 8;
                    }
                    // not tested (todo: test it!)
                    else if (ev.get_status() == EVENT_MIDI_SONG_POS)
                    {
                        unsigned char a, b;
                        ev.get_data(&a, &b);
                        m_midiclockpos = ((int)a << 7) && (int)b;
                    }

                    /* filter system wide messages */
                    if (ev.get_status() <= EVENT_SYSEX) {

                        if( global_showmidi)
                            ev.print();

                        /* is there a sequence set ? */
                        if (m_master_bus.is_dumping()) {

                            ev.set_timestamp(m_tick);


                            /* dump to it */
                            (m_master_bus.get_sequence())->stream_event(&ev);

                        }

                        /* use it to control our sequencer */
                        else {

                            for (int i = 0; i < c_midi_controls; i++) {

                                unsigned char data[2] = {0,0};
                                unsigned char status = ev.get_status();

                                ev.get_data( &data[0], &data[1] );

                                if (get_midi_control_toggle(i)->m_active &&
                                        status  == get_midi_control_toggle(i)->m_status &&
                                        data[0] == get_midi_control_toggle(i)->m_data ){

                                    if (data[1] >= get_midi_control_toggle(i)->m_min_value &&
                                            data[1] <= get_midi_control_toggle(i)->m_max_value ){

                                        if ( i <  c_seqs_in_set )
                                            sequence_playing_toggle( i + m_offset );
                                    }
                                }

                                if ( get_midi_control_on(i)->m_active &&
                                        status  == get_midi_control_on(i)->m_status &&
                                        data[0] == get_midi_control_on(i)->m_data ){

                                    if ( data[1] >= get_midi_control_on(i)->m_min_value &&
                                            data[1] <= get_midi_control_on(i)->m_max_value ){

                                        if ( i <  c_seqs_in_set )
                                            sequence_playing_on( i  + m_offset);
                                        else
                                            handle_midi_control( i, true );

                                    } else if (  get_midi_control_on(i)->m_inverse_active ){

                                        if ( i <  c_seqs_in_set )
                                            sequence_playing_off(  i + m_offset );
                                        else
                                            handle_midi_control( i, false );
                                    }

                                }

                                if ( get_midi_control_off(i)->m_active &&
                                        status  == get_midi_control_off(i)->m_status &&
                                        data[0] == get_midi_control_off(i)->m_data ){

                                    if ( data[1] >= get_midi_control_off(i)->m_min_value &&
                                            data[1] <= get_midi_control_off(i)->m_max_value ){

                                        if ( i <  c_seqs_in_set )
                                            sequence_playing_off(  i + m_offset );
                                        else
                                            handle_midi_control( i, false );

                                    } else if ( get_midi_control_off(i)->m_inverse_active ){

                                        if ( i <  c_seqs_in_set )
                                            sequence_playing_on(  i + m_offset );
                                        else
                                            handle_midi_control( i, true );

                                    }

                                }

                            }
                        }

                    }

                    if (ev.get_status() == EVENT_SYSEX) {

                        if (global_showmidi)
                            ev.print();

                        if (global_pass_sysex)
                            m_master_bus.sysex(&ev);
                    }
                }

            } while (m_master_bus.is_more_input());
        }
    }
    pthread_exit(0);
}


void MidiPerformance::save_playing_state()
{
    for( int i=0; i<c_total_seqs; i++ ){

        if ( is_active(i) == true ){
            assert( m_seqs[i] );
            m_sequence_state[i] = m_seqs[i]->get_playing();
        }
        else
            m_sequence_state[i] = false;
    }
}


void MidiPerformance::restore_playing_state()
{
    for( int i=0; i<c_total_seqs; i++ ){

        if ( is_active(i) == true ){
            assert( m_seqs[i] );
            m_seqs[i]->set_playing( m_sequence_state[i] );
        }
    }
}

void MidiPerformance::set_sequence_control_status( int a_status )
{
    if ( a_status & c_status_snapshot ){
        save_playing_state(  );
    }

    m_control_status |= a_status;
}


void MidiPerformance::unset_sequence_control_status( int a_status )
{
    if ( a_status & c_status_snapshot ){
        restore_playing_state(  );
    }

    m_control_status &= (~a_status);
}


void MidiPerformance::sequence_playing_toggle( int a_sequence )
{
    if ( is_active(a_sequence) == true ){
        assert( m_seqs[a_sequence] );

        if ( m_control_status & c_status_queue ){
            m_seqs[a_sequence]->toggle_queued();
        }

        else {

            if (  m_control_status & c_status_replace ){
                unset_sequence_control_status( c_status_replace );
                off_sequences( );
            }
            m_seqs[a_sequence]->toggle_playing();
        }

        /* if we're in song playback, temporarily block the events
         * till the next seq boundary */
        if (m_playback_mode){
            m_seqs[a_sequence]->set_song_playback_block(true);
        }
    }
}

void MidiPerformance::sequence_playing_on( int a_sequence )
{
    if ( is_active(a_sequence) == true ){
        if (m_mode_group && (m_playing_screen == m_screen_set)
                && (a_sequence >= (m_playing_screen * c_seqs_in_set))
                && (a_sequence < ((m_playing_screen + 1) * c_seqs_in_set)))
            m_tracks_mute_state[a_sequence - m_playing_screen * c_seqs_in_set] = true;
        assert( m_seqs[a_sequence] );
        if (!(m_seqs[a_sequence]->get_playing())) {
            if (m_control_status & c_status_queue ) {
                if (!(m_seqs[a_sequence]->get_queued()))
                    m_seqs[a_sequence]->toggle_queued();
            } else
                m_seqs[a_sequence]->set_playing(true);
        } else {
            if ((m_seqs[a_sequence]->get_queued()) && (m_control_status & c_status_queue ))
                m_seqs[a_sequence]->toggle_queued();
        }
    }
}


void MidiPerformance::sequence_playing_off( int a_sequence )
{
    if ( is_active(a_sequence) == true ){
        if (m_mode_group && (m_playing_screen == m_screen_set)
                && (a_sequence >= (m_playing_screen * c_seqs_in_set))
                && (a_sequence < ((m_playing_screen + 1) * c_seqs_in_set)))
            m_tracks_mute_state[a_sequence - m_playing_screen * c_seqs_in_set] = false;
        assert( m_seqs[a_sequence] );
        if (m_seqs[a_sequence]->get_playing()) {
            if (m_control_status & c_status_queue ) {
                if (!(m_seqs[a_sequence]->get_queued()))
                    m_seqs[a_sequence]->toggle_queued();
            } else
                m_seqs[a_sequence]->set_playing(false);
        } else {
            if ((m_seqs[a_sequence]->get_queued()) && (m_control_status & c_status_queue ))
                m_seqs[a_sequence]->toggle_queued();
        }
    }
}


void MidiPerformance::set_key_event( unsigned int keycode, long sequence_slot )
{
    // unhook previous binding...
    std::map<unsigned int,long>::iterator it1 = key_events.find( keycode );
    if (it1 != key_events.end())
    {
        std::map<long,unsigned int>::iterator i = key_events_rev.find( it1->second );
        if (i != key_events_rev.end())
            key_events_rev.erase( i );
        key_events.erase( it1 );
    }
    std::map<long,unsigned int>::iterator it2 = key_events_rev.find( sequence_slot );
    if (it2 != key_events_rev.end())
    {
        std::map<unsigned int,long>::iterator i = key_events.find( it2->second );
        if (i != key_events.end())
            key_events.erase( i );
        key_events_rev.erase( it2 );
    }
    // set
    key_events[keycode] = sequence_slot;
    key_events_rev[sequence_slot] = keycode;
}


void MidiPerformance::set_key_group( unsigned int keycode, long group_slot )
{
    // unhook previous binding...
    std::map<unsigned int,long>::iterator it1 = key_groups.find( keycode );
    if (it1 != key_groups.end())
    {
        std::map<long,unsigned int>::iterator i = key_groups_rev.find( it1->second );
        if (i != key_groups_rev.end())
            key_groups_rev.erase( i );
        key_groups.erase( it1 );
    }
    std::map<long,unsigned int>::iterator it2 = key_groups_rev.find( group_slot );
    if (it2 != key_groups_rev.end())
    {
        std::map<unsigned int,long>::iterator i = key_groups.find( it2->second );
        if (i != key_groups.end())
            key_groups.erase( i );
        key_groups_rev.erase( it2 );
    }
    // set
    key_groups[keycode] = group_slot;
    key_groups_rev[group_slot] = keycode;
}


#ifdef JACK_SUPPORT
void jack_timebase_callback(jack_transport_state_t state,
        jack_nframes_t nframes,
        jack_position_t *pos, int new_pos, void *arg)
{
    static double jack_tick;
    //static jack_nframes_t last_frame;
    static jack_nframes_t current_frame;
    static jack_transport_state_t state_current;
    static jack_transport_state_t state_last;

    state_current = state;

    MidiPerformance *p = (MidiPerformance *) arg;
    current_frame = jack_get_current_transport_frame( p->m_jack_client );

    //printf( "jack_timebase_callback() [%d] [%d] [%d]", state, new_pos, current_frame);

    pos->valid = JackPositionBBT;
    pos->beats_per_bar = 4;
    pos->beat_type = 4;
    pos->ticks_per_beat = c_ppqn * 10;
    pos->beats_per_minute = p->get_bpm();


    /* Compute BBT info from frame number.  This is relatively
     * simple here, but would become complex if we supported tempo
     * or time signature changes at specific locations in the
     * transport timeline. */

    // if we are in a new position
    if (  state_last    ==  JackTransportStarting &&
            state_current ==  JackTransportRolling ){

        // printf ( "Starting [%d] [%d]\n", last_frame, current_frame );

		 double jack_delta_tick =
            (current_frame) *
            pos->ticks_per_beat *
            pos->beats_per_minute / (pos->frame_rate * 60.0);
		
		jack_tick = (jack_delta_tick < 0) ? -jack_delta_tick : jack_delta_tick;
		//last_frame = current_frame;
		
		long ptick = 0, pbeat = 0, pbar = 0;
	
		pbar  = (long) ((long) jack_tick / (pos->ticks_per_beat *  pos->beats_per_bar ));

		pbeat = (long) ((long) jack_tick % (long) (pos->ticks_per_beat *  pos->beats_per_bar ));
		pbeat = pbeat / (long) pos->ticks_per_beat;

		ptick = (long) jack_tick % (long) pos->ticks_per_beat;

		// printf( " bbb [%2d:%2d:%4d]\n", pos->bar, pos->beat, pos->tick );

		pos->bar = pbar + 1;
		pos->beat = pbeat + 1;
		pos->tick = ptick;
		pos->bar_start_tick = pos->bar * pos->beats_per_bar *
			pos->ticks_per_beat;
    }

    state_last = state_current;
}


void jack_shutdown(void *arg)
{
    MidiPerformance *p = (MidiPerformance *) arg;
    p->m_jack_running = false;

    printf("JACK shut down.\nJACK sync Disabled.\n");
}


void print_jack_pos( jack_position_t* jack_pos ){

    return;
    printf( "print_jack_pos()\n" );
    printf( "    bar  [%d]\n", jack_pos->bar  );
    printf( "    beat [%d]\n", jack_pos->beat );		
    printf( "    tick [%d]\n", jack_pos->tick );
    printf( "    bar_start_tick   [%lf]\n", jack_pos->bar_start_tick );
    printf( "    beats_per_bar    [%f]\n", jack_pos->beats_per_bar );
    printf( "    beat_type        [%f]\n", jack_pos->beat_type );
    printf( "    ticks_per_beat   [%lf]\n", jack_pos->ticks_per_beat );
    printf( "    beats_per_minute [%lf]\n", jack_pos->beats_per_minute );
    printf( "    frame_time       [%lf]\n", jack_pos->frame_time );
    printf( "    next_time        [%lf]\n", jack_pos->next_time );
}


#if 0

int main ()
{
    jack_client_t *client;

    /* become a new client of the JACK server */
    if ((client = jack_client_new("transport tester")) == 0) {
        fprintf(stderr, "jack server not running?\n");
        return 1;
    }

    jack_on_shutdown(client, jack_shutdown, 0);
    jack_set_sync_callback(client, jack_sync_callback, NULL);

    if (jack_activate(client)) {
        fprintf(stderr, "cannot activate client");
        return 1;
    }

    bool cond = false; /* true if we want to fail if there is already a master */
    if (jack_set_timebase_callback(client, cond, timebase, NULL) != 0){
        printf("Unable to take over timebase or there is already a master.\n");
        exit(1);
    }

    jack_position_t pos;

    pos.valid = JackPositionBBT;

    pos.bar = 0;
    pos.beat = 0;
    pos.tick = 0;

    pos.beats_per_bar = time_beats_per_bar;
    pos.beat_type = time_beat_type;
    pos.ticks_per_beat = time_ticks_per_beat;
    pos.beats_per_minute = time_beats_per_minute;
    pos.bar_start_tick = 0.0;


    //jack_transport_reposition( client, &pos );

    jack_transport_start (client);

    //void jack_transport_stop (jack_client_t *client);

    int bob;
    scanf ("%d", &bob);

    jack_transport_stop (client);
    jack_release_timebase(client);
    jack_client_close(client);

    return 0;
}

#endif


#endif
