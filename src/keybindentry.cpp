//----------------------------------------------------------------------------
//
//  This file is part of seq24.
//
//  seq24 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  seq24 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with seq24; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//-----------------------------------------------------------------------------

// GTK text edit widget for getting keyboard button values (for binding keys)
// put cursor in text box, hit a key, something like  'a' (42)  appears...
// each keypress replaces the previous text.
// also supports keyevent and keygroup maps in the perform class

#include "keybindentry.h"
#include "perform.h"


KeyBindEntry::KeyBindEntry(type t, unsigned int* location_to_write,
        perform* p, long s):
    Entry(),
    m_key( location_to_write ),
    m_type( t ),
    m_perf( p ),
    m_slot( s )
{
    switch (m_type)
    {
        case location: if (m_key) set( *m_key ); break;
        case events: set( m_perf->lookup_keyevent_key( m_slot ) ); break;
        case groups: set( m_perf->lookup_keygroup_key( m_slot ) ); break;
    }
}

void KeyBindEntry::set( unsigned int val )
{
    char buf[256] = "";
    char* special = gdk_keyval_name( val );
    char* p_buf = &buf[strlen(buf)];

    if (special)
        snprintf( p_buf, sizeof buf - (p_buf - buf), "%s", special );
    else
        snprintf( p_buf, sizeof buf - (p_buf - buf), "'%c'", (char)val );
    
    set_text( buf );
    int width = strlen(buf)-1;
    set_width_chars( 1 <= width ? width : 1 );
}

bool KeyBindEntry::on_key_press_event(GdkEventKey* event)
{
    bool result = Entry::on_key_press_event( event );
    set( event->keyval );
    
    switch (m_type)
    {
        case location: if (m_key) *m_key = event->keyval; break;
        case events: m_perf->set_key_event( event->keyval, m_slot ); break;
        case groups: m_perf->set_key_group( event->keyval, m_slot ); break;
    }
    return result;
}

