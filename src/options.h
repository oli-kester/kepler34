/* user preferences dialog */

#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/box.h>
#include <gtkmm/main.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/window.h>
#include <gtkmm/dialog.h>
#include <gtkmm/table.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/fileselection.h>
#include <gtkmm/dialog.h>
#include <gtkmm/arrow.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/notebook.h>
#include <gtkmm/tooltips.h>

#include <sigc++/bind.h>

#include "globals.h"
#include "perform.h"

using namespace Gtk;

class options : public Gtk::Dialog
{
 private:

#if GTK_MINOR_VERSION < 12
    Tooltips *m_tooltips;
#endif

    perform *m_perf;

    Button  *m_button_ok;
    Label* interaction_method_label;
    Label* interaction_method_desc_label;

    Table   *m_table;

    Notebook *m_notebook;

    enum button {
        e_jack_transport,
        e_jack_master,
        e_jack_master_cond,
        e_jack_start_mode_live,
        e_jack_start_mode_song,
        e_jack_connect,
        e_jack_disconnect
    };

    void clock_callback_off( int a_bus, RadioButton *a_button );
    void clock_callback_on ( int a_bus, RadioButton *a_button );
    void clock_callback_mod( int a_bus, RadioButton *a_button );

    void clock_mod_callback( Adjustment *adj );

    void input_callback( int a_bus, Button *a_button );

    void transport_callback( button a_type, Button *a_button );

    void mouse_seq24_callback(Gtk::RadioButton*);
    void mouse_fruity_callback(Gtk::RadioButton*);

    /*notebook pages*/
    void add_midi_clock_page();
    void add_midi_input_page();
    void add_keyboard_page();
    void add_mouse_page();
    void add_jack_sync_page();

public:

    options( Gtk::Window &parent, perform *a_p );
    void add_recent_file( string path );
};

