/* reads/writes user preferences to .kepler34rc file */

#include <iostream>
#include <fstream>
#include <string>

#include "PreferencesFile.hpp"

extern QString last_used_dir;

PreferencesFile::PreferencesFile(const QString& a_name) :
    ConfigFile( a_name )
{

}

bool
PreferencesFile::parse( MidiPerformance *a_perf )
{
    /* open binary file */
    ifstream file ( m_name.toUtf8().constData(), ios::in | ios::ate );

    if( ! file.is_open() )
        return false;

    /* run to start */
    file.seekg( 0, ios::beg );

    line_after( &file, "[midi-control]" );

    unsigned int sequences = 0;
    sscanf( m_line, "%u", &sequences );
    next_data_line( &file );

    for (unsigned int i = 0; i < sequences; ++i) {

        int sequence = 0;

        sscanf(m_line, "%d [ %d %d %ld %ld %ld %ld ]"
                       " [ %d %d %ld %ld %ld %ld ]"
                       " [ %d %d %ld %ld %ld %ld ]",

               &sequence,

               &a_perf->get_midi_control_toggle(i)->m_active,
               &a_perf->get_midi_control_toggle(i)->m_inverse_active,
               &a_perf->get_midi_control_toggle(i)->m_status,
               &a_perf->get_midi_control_toggle(i)->m_data,
               &a_perf->get_midi_control_toggle(i)->m_min_value,
               &a_perf->get_midi_control_toggle(i)->m_max_value,

               &a_perf->get_midi_control_on(i)->m_active,
               &a_perf->get_midi_control_on(i)->m_inverse_active,
               &a_perf->get_midi_control_on(i)->m_status,
               &a_perf->get_midi_control_on(i)->m_data,
               &a_perf->get_midi_control_on(i)->m_min_value,
               &a_perf->get_midi_control_on(i)->m_max_value,

               &a_perf->get_midi_control_off(i)->m_active,
               &a_perf->get_midi_control_off(i)->m_inverse_active,
               &a_perf->get_midi_control_off(i)->m_status,
               &a_perf->get_midi_control_off(i)->m_data,
               &a_perf->get_midi_control_off(i)->m_min_value,
               &a_perf->get_midi_control_off(i)->m_max_value);

        next_data_line(&file);
    }
    /* group midi control */
    line_after( &file, "[mute-group]");

    int gtrack = 0;
    sscanf( m_line, "%d", &gtrack );
    next_data_line( &file );

    int mtx[c_seqs_in_set], j=0;
    for (int i=0; i< c_seqs_in_set; i++) {
        a_perf->select_group_mute(j);
        sscanf(m_line, "%d [%d %d %d %d %d %d %d %d]"
                       " [%d %d %d %d %d %d %d %d]"
                       " [%d %d %d %d %d %d %d %d]"
                       " [%d %d %d %d %d %d %d %d]",
               &j,
               &mtx[0], &mtx[1], &mtx[2], &mtx[3],
                &mtx[4], &mtx[5], &mtx[6], &mtx[7],

                &mtx[8], &mtx[9], &mtx[10], &mtx[11],
                &mtx[12], &mtx[13], &mtx[14], &mtx[15],

                &mtx[16], &mtx[17], &mtx[18], &mtx[19],
                &mtx[20], &mtx[21], &mtx[22], &mtx[23],

                &mtx[24], &mtx[25], &mtx[26], &mtx[27],
                &mtx[28], &mtx[29], &mtx[30], &mtx[31]);
        for (int k=0; k< c_seqs_in_set; k++) {
            a_perf->set_group_mute_state(k, mtx[k]);
        }
        j++;
        next_data_line( &file );
    }

    line_after( &file, "[midi-clock]" );
    long buses = 0;
    sscanf( m_line, "%ld", &buses );
    next_data_line( &file );

    for ( int i=0; i<buses; ++i ){

        long bus_on, bus;
        sscanf( m_line, "%ld %ld", &bus, &bus_on );
        a_perf->get_master_midi_bus( )->set_clock( bus, (clock_e) bus_on );
        next_data_line( &file );
    }


    line_after( &file, "[keyboard-control]" );
    long keys = 0;
    sscanf( m_line, "%ld", &keys );
    next_data_line( &file );

    //only clear default key bindings
    //if we have replacements
    if (keys > 0)
    {
        a_perf->key_events.clear();

        for ( int i=0; i<keys; ++i )
        {
            long key = 0, seq = 0;
            sscanf( m_line, "%ld %ld", &key, &seq );
            a_perf->set_key_event( key, seq );
            next_data_line( &file );
        }
    }

    line_after( &file, "[keyboard-group]" );
    long groups = 0;
    sscanf( m_line, "%ld", &groups );
    next_data_line( &file );

    if (groups > 0)
    {
        a_perf->key_groups.clear();

        for ( int i=0; i<groups; ++i )
        {
            long key = 0, group = 0;
            sscanf( m_line, "%ld %ld", &key, &group );
            a_perf->set_key_group( key, group );
            next_data_line( &file );
        }
    }

    sscanf( m_line, "%u %u", &a_perf->m_key_bpm_up,
            &a_perf->m_key_bpm_dn );
    next_data_line( &file );
    sscanf( m_line, "%u %u %u", &a_perf->m_key_screenset_up,
            &a_perf->m_key_screenset_dn,
            &a_perf->m_key_set_playing_screenset);


    next_data_line( &file );

    sscanf( m_line, "%u %u %u", &a_perf->m_key_group_on,
            &a_perf->m_key_group_off,
            &a_perf->m_key_group_learn);

    next_data_line( &file );


    sscanf( m_line, "%s %s %s %s %s",
            &a_perf->m_key_replace,
            &a_perf->m_key_queue,
            &a_perf->m_key_snapshot_1,
            &a_perf->m_key_snapshot_2,
            &a_perf->m_key_keep_queue);
    next_data_line( &file );



    int show_key = 0;
    sscanf(m_line, "%d", &show_key);
    a_perf->m_show_ui_sequence_key = (bool) show_key;
    next_data_line( &file );

    sscanf( m_line, "%u", &a_perf->m_key_start );
    next_data_line( &file );

    sscanf( m_line, "%u", &a_perf->m_key_stop );

    line_after( &file, "[jack-transport]" );
    long flag = 0;

    sscanf( m_line, "%ld", &flag );
    global_with_jack_transport = (bool) flag;

    next_data_line( &file );
    sscanf( m_line, "%ld", &flag );
    global_with_jack_master = (bool) flag;

    next_data_line( &file );
    sscanf( m_line, "%ld", &flag );
    global_with_jack_master_cond = (bool) flag;

    line_after( &file, "[midi-input]" );
    buses = 0;
    sscanf( m_line, "%ld", &buses );
    next_data_line( &file );

    for ( int i=0; i<buses; ++i ){

        long bus_on, bus;
        sscanf( m_line, "%ld %ld", &bus, &bus_on );
        a_perf->get_master_midi_bus( )->set_input( bus, (bool) bus_on );
        next_data_line( &file );
    }

    /* midi clock mod */
    long ticks = 64;
    line_after( &file, "[midi-clock-mod-ticks]" );
    sscanf( m_line, "%ld", &ticks );
    MidiBus::set_clock_mod(ticks);


    /* manual alsa ports */
    line_after( &file, "[manual-alsa-ports]" );
    sscanf( m_line, "%ld", &flag );
    global_manual_alsa_ports = (bool) flag;

    /* last used dir */
    line_after( &file, "[last-used-dir]" );
    //FIXME: check for a valid path is missing
    if (m_line[0] == '/')
        last_used_dir = m_line;

    /* recent files list */
    line_after( &file, "[recent-files]" );
    for (int c = 0;c<10;c++){
        if (m_line[0]!=EOF){
            recent_files[c] = m_line;
        } else
            recent_files[c] = "";
        next_data_line(&file);
    }

    /* interaction method  */
    long method = 0;
    line_after( &file, "[interaction-method]" );
    sscanf( m_line, "%ld", &method );
    global_interactionmethod = (interaction_method_e)method;

    file.close();

    return true;
}


bool
PreferencesFile::write( MidiPerformance *a_perf  )
{
    /* open binary file */

    ofstream file ( m_name.toUtf8().constData(), ios::out | ios::trunc  );
    char outs[1024];

    if( ! file.is_open() )
        return false;

    /* midi control */

    file << "#\n";
    file << "#Kepler34 Init File\n";
    file << "#\n\n\n";

    file << "[midi-control]\n";
    file <<  c_midi_controls << "\n";

    for (int i=0; i< c_midi_controls; i++ ){


        switch( i ){

        /* 32 mute for channel
               32 group mute */
        case c_seqs_in_set               :  file << "# mute in group\n"; break;
        case c_midi_control_bpm_up       :  file << "# bpm up\n"; break;
        case c_midi_control_bpm_dn       :  file << "# bpm down\n"; break;
        case c_midi_control_ss_up        :  file << "# screen set up\n"; break;
        case c_midi_control_ss_dn        :  file << "# screen set down\n"; break;
        case c_midi_control_mod_replace  :  file << "# mod replace\n"; break;
        case c_midi_control_mod_snapshot :  file << "# mod snapshot\n"; break;
        case c_midi_control_mod_queue    :  file << "# mod queue\n"; break;
        case c_midi_control_mod_gmute    :  file << "# mod gmute\n"; break;
        case c_midi_control_mod_glearn   :  file << "# mod glearn\n"; break;
        case c_midi_control_play_ss      :  file << "# screen set play\n"; break;


        default: break;
        }

        snprintf( outs, sizeof(outs), "%d [%1d %1d %3ld %3ld %3ld %3ld]"
                                      " [%1d %1d %3ld %3ld %3ld %3ld]"
                                      " [%1d %1d %3ld %3ld %3ld %3ld]",
                  i,
                  a_perf->get_midi_control_toggle(i)->m_active,
                  a_perf->get_midi_control_toggle(i)->m_inverse_active,
                  a_perf->get_midi_control_toggle(i)->m_status,
                  a_perf->get_midi_control_toggle(i)->m_data,
                  a_perf->get_midi_control_toggle(i)->m_min_value,
                  a_perf->get_midi_control_toggle(i)->m_max_value,

                  a_perf->get_midi_control_on(i)->m_active,
                  a_perf->get_midi_control_on(i)->m_inverse_active,
                  a_perf->get_midi_control_on(i)->m_status,
                  a_perf->get_midi_control_on(i)->m_data,
                  a_perf->get_midi_control_on(i)->m_min_value,
                  a_perf->get_midi_control_on(i)->m_max_value,

                  a_perf->get_midi_control_off(i)->m_active,
                  a_perf->get_midi_control_off(i)->m_inverse_active,
                  a_perf->get_midi_control_off(i)->m_status,
                  a_perf->get_midi_control_off(i)->m_data,
                  a_perf->get_midi_control_off(i)->m_min_value,
                  a_perf->get_midi_control_off(i)->m_max_value );


        file << string(outs) << "\n";
    }

    /* group midi control */
    file << "\n\n\n[mute-group]\n";

    int mtx[c_seqs_in_set];
    file <<  c_gmute_tracks << "\n";
    for (int j=0; j < c_seqs_in_set; j++ ){
        a_perf->select_group_mute(j);
        for (int i=0; i < c_seqs_in_set; i++) {
            mtx[i] = a_perf->get_group_mute_state(i);
        }
        snprintf(outs, sizeof(outs),
                 "%d [%1d %1d %1d %1d %1d %1d %1d %1d]"
                 " [%1d %1d %1d %1d %1d %1d %1d %1d]"
                 " [%1d %1d %1d %1d %1d %1d %1d %1d]"
                 " [%1d %1d %1d %1d %1d %1d %1d %1d]",
                 j,
                 mtx[0], mtx[1], mtx[2], mtx[3],
                mtx[4], mtx[5], mtx[6], mtx[7],

                mtx[8], mtx[9], mtx[10], mtx[11],
                mtx[12], mtx[13], mtx[14], mtx[15],

                mtx[16], mtx[17], mtx[18], mtx[19],
                mtx[20], mtx[21], mtx[22], mtx[23],

                mtx[24], mtx[25], mtx[26], mtx[27],
                mtx[28], mtx[29], mtx[30], mtx[31]);

        file << string(outs) << "\n";
    }



    /* bus mute/unmute data */
    int buses = a_perf->get_master_midi_bus( )->get_num_out_buses();

    file << "\n\n\n[midi-clock]\n";
    file << buses << "\n";

    for (int i=0; i< buses; i++ ){


        file << "# " << a_perf->get_master_midi_bus( )->get_midi_out_bus_name(i) << "\n";
        snprintf(outs, sizeof(outs), "%d %d", i,
                 (char) a_perf->get_master_midi_bus( )->get_clock(i));
        file << outs << "\n";
    }

    /* midi clock mod  */
    file << "\n\n[midi-clock-mod-ticks]\n";
    file << MidiBus::get_clock_mod() << "\n";

    /* bus input data */
    buses = a_perf->get_master_midi_bus( )->get_num_in_buses();

    file << "\n\n\n[midi-input]\n";
    file << buses << "\n";

    for (int i=0; i< buses; i++ ){


        file << "# " << a_perf->get_master_midi_bus( )->get_midi_in_bus_name(i) << "\n";
        snprintf(outs, sizeof(outs), "%d %d", i,
                 (char) a_perf->get_master_midi_bus( )->get_input(i));
        file << outs << "\n";
    }


    /* manual alsa ports */
    file << "\n\n\n[manual-alsa-ports]\n";
    file << "# set to 1 if you want seq24 to create its own alsa ports and\n";
    file << "# not connect to other clients\n";
    file << global_manual_alsa_ports << "\n";

    /* interaction-method */
    int x = 0;
    file << "\n\n\n[interaction-method]\n";
    while (c_interaction_method_names[x] && c_interaction_method_descs[x])
    {
        file << "# " << x << " - '" << c_interaction_method_names[x]
                << "' (" << c_interaction_method_descs[x] << ")\n";
        ++x;
    }
    file << global_interactionmethod << "\n";



    file << "\n\n\n[keyboard-control]\n";
    file << "# Key #, Sequence # \n";
    file << (a_perf->key_events.size() < (size_t)c_seqs_in_set ?
                 a_perf->key_events.size() : (size_t)c_seqs_in_set) << "\n";

    for (std::map<int,long>::const_iterator i = a_perf->key_events.begin();
         i != a_perf->key_events.end(); ++i )
    {
        qDebug() << "Writing control mapping - " << i->first << " mapped to " << i->second << "Keyname - " << QKeySequence(i->first).toString().toStdString().c_str() << endl;

        snprintf(outs, sizeof(outs), "%u  %ld        # %s", i->first,
                 i->second, QKeySequence(i->first).toString().toStdString().c_str());

        file << string(outs) << "\n";
    }

    file << "\n\n\n[keyboard-group]\n";
    file << "# Key #, group # \n";
    file << (a_perf->key_groups.size() < (size_t)c_seqs_in_set ?
                 a_perf->key_groups.size() : (size_t)c_seqs_in_set) << "\n";

    for( std::map<int,long>::const_iterator i = a_perf->key_groups.begin();
         i != a_perf->key_groups.end(); ++i )
    {
        qDebug() << "Writing group mapping - " << i->first << " mapped to " << i->second << "Keyname - " << QKeySequence(i->first).toString().toStdString().c_str() << endl;

        snprintf(outs, sizeof(outs), "%u  %ld        # %s", i->first,
                 i->second, QKeySequence(i->first).toString().toStdString().c_str());

        file << string(outs) << "\n";
    }

    file << "\n\n# bpm up, down\n"
         << a_perf->m_key_bpm_up << " "
         << a_perf->m_key_bpm_dn << "        # "
         << QKeySequence(a_perf->m_key_bpm_up).toString().toStdString().c_str() << " "
         << QKeySequence(a_perf->m_key_bpm_dn).toString().toStdString().c_str() << "\n";

    file << "# screen set up, down, play\n"
         << a_perf->m_key_screenset_up << " "
         << a_perf->m_key_screenset_dn << " "
         << a_perf->m_key_set_playing_screenset << "        # "
         << QKeySequence(a_perf->m_key_screenset_up).toString().toStdString().c_str() << " "
         << QKeySequence(a_perf->m_key_screenset_dn).toString().toStdString().c_str() << " "
         << QKeySequence(a_perf->m_key_set_playing_screenset).toString().toStdString().c_str() << "\n";

    file << "# group on, off, learn\n"
         << a_perf->m_key_group_on << " "
         << a_perf->m_key_group_off << " "
         << a_perf->m_key_group_learn << "        # "
         << QKeySequence(a_perf->m_key_group_on).toString().toStdString().c_str() << " "
         << QKeySequence(a_perf->m_key_group_off).toString().toStdString().c_str() << " "
         << QKeySequence(a_perf->m_key_group_learn).toString().toStdString().c_str() << "\n";

//    qDebug() << a_perf->m_key_replace.toString(QKeySequence::NativeText) << endl;

    file << "# replace, queue, snapshot_1, snapshot 2, keep queue\n"
         << a_perf->m_key_replace << " "
         << a_perf->m_key_queue << " "
         << a_perf->m_key_snapshot_1 << " "
         << a_perf->m_key_snapshot_2 << " "
         << a_perf->m_key_keep_queue << "        # "
         << a_perf->m_key_replace << " "
         << a_perf->m_key_queue << " "
         << a_perf->m_key_snapshot_1 << " "
         << a_perf->m_key_snapshot_2 << " "
         << a_perf->m_key_keep_queue << "\n";

    file << a_perf->m_show_ui_sequence_key
         << "        # show_ui_sequence_key (1=true/0=false)\n";
    file << a_perf->m_key_start << "        # "
         << QKeySequence(a_perf->m_key_start).toString().toStdString().c_str()
         << " start sequencer\n";
    file << a_perf->m_key_stop << "        # "
         << QKeySequence(a_perf->m_key_stop).toString().toStdString().c_str()
         << " stop sequencer\n";

    file << "\n\n\n[jack-transport]\n\n"


         << "# jack_transport - Enable sync with JACK Transport.\n"
         << global_with_jack_transport << "\n\n"

         << "# jack_master - Seq24 will attempt to serve as JACK Master.\n"
         << global_with_jack_master << "\n\n"

         << "# jack_master_cond -  Seq24 will fail to be master if there is already a master set.\n"
         << global_with_jack_master_cond  << "\n\n";

    file << "\n\n\n[last-used-dir]\n\n"
         << "# Last used directory.\n"
         << last_used_dir.toStdString().c_str() << "\n\n";

    file << "\n\n\n[recent-files]\n\n"
         << "# List of 10 recently opened files.\n";
    for (int c=0;c<10;c++){
        file << recent_files[c].toStdString().c_str() << "\n";
    }

    file.close();
    return true;
}
