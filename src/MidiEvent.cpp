#include "MidiEvent.hpp"
#include "string.h"

MidiEvent::MidiEvent() :
    m_timestamp(0),
    m_status(EVENT_NOTE_OFF),
    m_linked(NULL),
    m_has_link(false),
    m_selected(false),
    m_marked(false),
    m_painted(false)
{
    m_data[0] = 0;
    m_data[1] = 0;
}

long
MidiEvent::get_timestamp()
{
    return m_timestamp;
}

void
MidiEvent::set_timestamp( const unsigned long a_time )
{
    m_timestamp = a_time;
}

void
MidiEvent::mod_timestamp( unsigned long a_mod )
{
    m_timestamp %= a_mod;
}

void
MidiEvent::set_status( const char a_status  )
{
   /* bitwise AND to clear the channel portion of the status */
    if ( (unsigned char) a_status >= 0xF0 )
      m_status = (char) a_status;
    else
      m_status = (char) (a_status & EVENT_CLEAR_CHAN_MASK);
}

void
MidiEvent::make_clock( )
{
    m_status = (unsigned char) EVENT_MIDI_CLOCK;
}

void
MidiEvent::set_data( char a_D1  )
{
    m_data[0] = a_D1 & 0x7F;
}

void
MidiEvent::set_data( char a_D1, char a_D2 )
{
    m_data[0] = a_D1 & 0x7F;
    m_data[1] = a_D2 & 0x7F;
}

void
MidiEvent::increment_data2()
{
	m_data[1] = (m_data[1]+1) & 0x7F;
}

void
MidiEvent::decrement_data2()
{
	m_data[1] = (m_data[1]-1) & 0x7F;
}



void
MidiEvent::increment_data1()
{
	m_data[0] = (m_data[0]+1) & 0x7F;
}

void
MidiEvent::decrement_data1()
{
	m_data[0] = (m_data[0]-1) & 0x7F;
}


void
MidiEvent::get_data( unsigned char *D0, unsigned char *D1 )
{
    *D0 = m_data[0];
    *D1 = m_data[1];
}

unsigned char
MidiEvent::get_status( )
{
    return m_status;
}


void
MidiEvent::start_sysex()
{
  m_sysex.clear();
}

bool
MidiEvent::append_sysex( unsigned char *a_data, long a_size )
{
  bool ret = true;

  for ( int i=0; i<a_size; i++ ){

    m_sysex.push_back( a_data[i] );
    if ( a_data[i] == EVENT_SYSEX_END )
      ret = false;
  }

  return ret;

}


unsigned char *
MidiEvent::get_sysex()
{
  return m_sysex.data();
}



void
MidiEvent::set_size( long a_size )
{
  m_sysex.resize(a_size);
}

long
MidiEvent::get_size()
{
  return m_sysex.size();
}

void
MidiEvent::set_note_velocity( int a_vel )
{
    m_data[1] = a_vel & 0x7F;
}

bool
MidiEvent::is_note_on()
{
    return (m_status == EVENT_NOTE_ON);
}

bool
MidiEvent::is_note_off()
{
    return (m_status == EVENT_NOTE_OFF);
}

unsigned char
MidiEvent::get_note()
{
    return m_data[0];
}

void
MidiEvent::set_note( char a_note )
{
    m_data[0] = a_note & 0x7F;
}

unsigned char
MidiEvent::get_note_velocity()
{
    return m_data[1];
}


void
MidiEvent::print()
{
    printf( "[%06ld] [%04lX] %02X ",
	    m_timestamp,
	    m_sysex.size(),
	    m_status );

    if ( m_status == EVENT_SYSEX ){

      for( size_t i=0; i<m_sysex.size(); i++ ){

	if ( i%16 == 0 )
	  printf( "\n    " );

	printf( "%02X ", m_sysex[i] );

      }

      printf( "\n" );
    }
    else {

      printf( "%02X %02X\n",
	      m_data[0],
	      m_data[1] );
    }
}

int
MidiEvent::get_rank(void) const
{
    switch ( m_status )
    {
        case EVENT_NOTE_OFF:
            return 0x100;
        case EVENT_NOTE_ON:
            return 0x090;

        case EVENT_AFTERTOUCH:
        case EVENT_CHANNEL_PRESSURE:
        case EVENT_PITCH_WHEEL:
            return 0x050;

        case EVENT_CONTROL_CHANGE:
            return 0x010;
        case EVENT_PROGRAM_CHANGE:
            return 0x000;
        default:
            return 0;
    }
}

bool
MidiEvent::operator>( const MidiEvent &a_rhsevent )
{
    if ( m_timestamp == a_rhsevent.m_timestamp )
    {
        return (get_rank() > a_rhsevent.get_rank());
    }
    else
    {
        return (m_timestamp > a_rhsevent.m_timestamp);
    }
}


bool
MidiEvent::operator<( const MidiEvent &a_rhsevent )
{
    if ( m_timestamp == a_rhsevent.m_timestamp )
    {
        return (get_rank() < a_rhsevent.get_rank());
    }
    else
    {
        return (m_timestamp < a_rhsevent.m_timestamp);
    }
}

bool
MidiEvent::operator<=( const unsigned long &a_rhslong )
{
    return (m_timestamp <= a_rhslong);
}



bool
MidiEvent::operator>( const unsigned long &a_rhslong )
{
    return (m_timestamp > a_rhslong);
}

void
MidiEvent::link( MidiEvent *a_event )
{
    m_has_link = true;
    m_linked = a_event;
}

MidiEvent*
MidiEvent::get_linked( )
{
    return m_linked;
}

bool
MidiEvent::is_linked( )
{
    return m_has_link;
}

void
MidiEvent::clear_link( )
{
    m_has_link = false;
}

void
MidiEvent::select( )
{
    m_selected = true;
}

void
MidiEvent::unselect( )
{
    m_selected = false;
}

bool
MidiEvent::is_selected( )
{
    return m_selected;
}
void
MidiEvent::paint( )
{
    m_painted = true;
}

void
MidiEvent::unpaint( )
{
    m_painted = false;
}

bool
MidiEvent::is_painted( )
{
    return m_painted;
}

void
MidiEvent::mark( )
{
    m_marked = true;
}

void
MidiEvent::unmark( )
{
    m_marked = false;
}

bool
MidiEvent::is_marked( )
{
    return m_marked;
}
