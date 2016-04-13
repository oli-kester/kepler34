#include "UserFile.hpp"
#include <iostream>

UserFile::UserFile( QString a_name ) :
    ConfigFile( a_name ) { }

UserFile::~UserFile( ) { }

bool
UserFile::parse( MidiPerformance *a_perf )
{

    /* open binary file */
    ifstream file ( m_name.toUtf8().constData(), ios::in | ios::ate );

    if( ! file.is_open() )
        return false;

    /* run to start */
    file.seekg( 0, ios::beg );

    line_after( &file, "[user-midi-bus-definitions]" );
    int buses = 0;
    sscanf( m_line, "%d", &buses );
    char bus_num[4];

    for ( int i=0; i<buses; i++ )
    {
        snprintf(bus_num, sizeof(bus_num), "%d", i);
        line_after( &file, "[user-midi-bus-" + string(bus_num) + "]");
        global_user_midi_bus_definitions[i].alias = m_line;
        next_data_line( &file );
        int instruments=0;
        int instrument;
        int channel;

        sscanf( m_line, "%d", &instruments );
        for ( int j=0; j<instruments; j++ )
        {
            next_data_line( &file );
            sscanf( m_line, "%d %d", &channel, &instrument );
            global_user_midi_bus_definitions[i].instrument[channel] = instrument;
        }
    }

    line_after( &file, "[user-instrument-definitions]" );
    int instruments = 0;
    sscanf( m_line, "%d", &instruments );
    char instrument_num[4];

    for ( int i=0; i<instruments; i++ )
    {
        snprintf(instrument_num, sizeof(instrument_num), "%d", i);
        line_after( &file, "[user-instrument-" + string(instrument_num) + "]");
        global_user_instrument_definitions[i].instrument = m_line;
        next_data_line( &file );
        int ccs=0;
        int cc=0;

        char cc_name[1024];

        sscanf( m_line, "%d", &ccs );
        for ( int j=0; j<ccs; j++ )
        {
            next_data_line( &file );
            sscanf( m_line, "%d", &cc );
            sscanf( m_line, "%[^\n]", cc_name );
            global_user_instrument_definitions[i].controllers[cc] = string(cc_name);
            global_user_instrument_definitions[i].controllers_active[cc] = true;
        }
    }

    file.close();

    return true;
}


bool
UserFile::write( MidiPerformance *a_perf  )
{
    return false;
}
