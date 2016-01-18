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

#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include <sstream>

#include "options.h"
#include "keybindentry.h"

// tooltip helper, for old vs new gtk...
#if GTK_MINOR_VERSION >= 12
#   define add_tooltip(obj, text) obj->set_tooltip_text(text);
#else
#   define add_tooltip(obj, text) m_tooltips->set_tip(*obj, text);
#endif

const int c_status = 0;
const int c_status_inv = 1;
const int c_d1 = 2;
const int c_d2 = 3;
const int c_d3 = 4;
enum {e_keylabelsonsequence = 9999};



options::options (Gtk::Window & parent, perform * a_p):
    Gtk::Dialog ("Options", parent, true, true),
    m_perf(a_p)
{
#if GTK_MINOR_VERSION < 12
    m_tooltips = manage(new Tooltips());
#endif

    HBox *hbox = manage (new HBox ());
    get_vbox ()->pack_start (*hbox, false, false);

    get_action_area ()->set_border_width (2);
    hbox->set_border_width (6);

    m_button_ok = manage (new Button(Gtk::Stock::OK));
    get_action_area ()->pack_end (*m_button_ok, false, false);
    m_button_ok->signal_clicked ().connect (mem_fun (*this, &options::hide));


    m_notebook = manage (new Notebook ());
    hbox->pack_start (*m_notebook);

    add_midi_clock_page();
    add_midi_input_page();
    add_keyboard_page();
    add_mouse_page();
    add_jack_sync_page();
}


/*MIDI Clock page*/
void
options::add_midi_clock_page()
{
    // Clock  Buses
    int buses = m_perf->get_master_midi_bus ()->get_num_out_buses ();

    VBox *vbox = manage(new VBox());
    vbox->set_border_width(6);
    m_notebook->append_page(*vbox, "MIDI _Clock", true);

    manage (new Tooltips ());

    for (int i = 0; i < buses; i++)
    {
        HBox *hbox2 = manage (new HBox ());
        Label *label = manage( new Label(m_perf->get_master_midi_bus ()->
                                            get_midi_out_bus_name (i), 0));

        hbox2->pack_start (*label, false, false);


        Gtk::RadioButton * rb_off = manage (new RadioButton ("Off"));
        add_tooltip( rb_off, "Midi Clock will be disabled.");

        Gtk::RadioButton * rb_on = manage (new RadioButton ("On (Pos)"));
        add_tooltip( rb_on,
                "Midi Clock will be sent. Midi Song Position and Midi Continue will be sent if starting greater than tick 0 in song mode, otherwise Midi Start is sent.");

        Gtk::RadioButton * rb_mod = manage (new RadioButton ("On (Mod)"));
        add_tooltip( rb_mod, "Midi Clock will be sent.  Midi Start will be sent and clocking will begin once the song position has reached the modulo of the specified Size. (Used for gear that doesn't respond to Song Position)");

        Gtk::RadioButton::Group group = rb_off->get_group ();
        rb_on->set_group (group);
        rb_mod->set_group (group);

        rb_off->signal_toggled().connect (sigc::bind(mem_fun (*this,
                        &options::clock_callback_off), i, rb_off ));
        rb_on->signal_toggled ().connect (sigc::bind(mem_fun (*this,
                        &options::clock_callback_on),  i, rb_on  ));
        rb_mod->signal_toggled().connect (sigc::bind(mem_fun (*this,
                        &options::clock_callback_mod), i, rb_mod ));

        hbox2->pack_end (*rb_mod, false, false );
        hbox2->pack_end (*rb_on, false, false);
        hbox2->pack_end (*rb_off, false, false);

        vbox->pack_start( *hbox2, false, false );

        switch ( m_perf->get_master_midi_bus ()->get_clock (i))
        {
            case e_clock_off: rb_off->set_active(1); break;
            case e_clock_pos: rb_on->set_active(1); break;
            case e_clock_mod: rb_mod->set_active(1); break;
        }
    }

    Adjustment *clock_mod_adj = new Adjustment(midibus::get_clock_mod(),
            1, 16 << 10, 1 );
    SpinButton *clock_mod_spin = new SpinButton( *clock_mod_adj );

    HBox *hbox2 = manage (new HBox ());

    //m_spinbutton_bpm->set_editable( false );
    hbox2->pack_start(*(manage(new Label(
                        "Clock Start Modulo (1/16 Notes)"))), false, false, 4);
    hbox2->pack_start(*clock_mod_spin, false, false );

    vbox->pack_start( *hbox2, false, false );

    clock_mod_adj->signal_value_changed().connect(sigc::bind(mem_fun(*this,
                    &options::clock_mod_callback), clock_mod_adj));
}


/*MIDI Input page*/
void
options::add_midi_input_page()
{
    // Input Buses
    int buses = m_perf->get_master_midi_bus ()->get_num_in_buses ();

    VBox *vbox = manage(new VBox ());
    vbox->set_border_width(6);
    m_notebook->append_page(*vbox, "MIDI _Input", true);

    for (int i = 0; i < buses; i++)
    {
        CheckButton *check = manage(new CheckButton(
                    m_perf->get_master_midi_bus()->get_midi_in_bus_name(i), 0));
        check->signal_toggled().connect(bind(mem_fun(*this,
                        &options::input_callback), i, check));
        check->set_active(m_perf->get_master_midi_bus()->get_input(i));

        vbox->pack_start(*check, false, false);
    }
}


/*Keyboard page*/
/*Keybinding setup (editor for .seq24rc keybindings).*/
void
options::add_keyboard_page()
{
    VBox *mainbox = manage(new VBox());
    mainbox->set_spacing(6);
    m_notebook->append_page(*mainbox, "_Keyboard", true);

    Label* label;
    KeyBindEntry* entry;

    HBox *hbox = manage (new HBox());
    CheckButton *check = manage(new CheckButton(
                "_Show key labels on sequences", true));
    check->signal_toggled().connect(bind(mem_fun(*this,
                    &options::input_callback), (int)e_keylabelsonsequence,
                check));
    check->set_active(m_perf->m_show_ui_sequence_key);
    mainbox->pack_start(*check, false, false);

    /*Frame for sequence toggle keys*/
    Frame* controlframe = manage(new Frame("Control keys"));
    controlframe->set_border_width(4);
    mainbox->pack_start(*controlframe, Gtk::PACK_SHRINK);

    Table* controltable = manage(new Table(4, 8, false));
    controltable->set_border_width(4);
    controltable->set_spacings(4);
    controlframe->add(*controltable);

    label = manage(new Label("Start", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_start));
    controltable->attach(*label, 0, 1, 0, 1);
    controltable->attach(*entry, 1, 2, 0, 1);

    label = manage(new Label("Stop", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_stop));
    controltable->attach(*label, 0, 1, 1, 2);
    controltable->attach(*entry, 1, 2, 1, 2);


    label = manage(new Label("Snapshot 1", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_snapshot_1));
    controltable->attach(*label, 2, 3, 0, 1);
    controltable->attach(*entry, 3, 4, 0, 1);

    label = manage(new Label("Snapshot 2", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_snapshot_2));
    controltable->attach(*label, 2, 3, 1, 2);
    controltable->attach(*entry, 3, 4, 1, 2);


    label = manage(new Label("bpm down", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_bpm_dn));
    controltable->attach(*label, 2, 3, 3, 4);
    controltable->attach(*entry, 3, 4, 3, 4);

    label = manage(new Label("bpm up", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_bpm_up));
    controltable->attach(*label, 2, 3, 2, 3);
    controltable->attach(*entry, 3, 4, 2, 3);


    label = manage(new Label("Replace", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_replace));
    controltable->attach(*label, 4, 5, 0, 1);
    controltable->attach(*entry, 5, 6, 0, 1);

    label = manage(new Label("Queue", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_queue));
    controltable->attach(*label, 4, 5, 1, 2);
    controltable->attach(*entry, 5, 6, 1, 2);

    label = manage(new Label("Keep queue", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_keep_queue));
    controltable->attach(*label, 4, 5, 2, 3);
    controltable->attach(*entry, 5, 6, 2, 3);


    label = manage(new Label("Screenset up", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_screenset_up));
    controltable->attach(*label, 6, 7, 0, 1);
    controltable->attach(*entry, 7, 8, 0, 1);

    label = manage(new Label("Screenset down", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_screenset_dn));
    controltable->attach(*label, 6, 7, 1, 2);
    controltable->attach(*entry, 7, 8, 1, 2);

    label = manage(new Label("Set playing screenset", Gtk::ALIGN_RIGHT));
    entry = manage(new KeyBindEntry(KeyBindEntry::location,
                &m_perf->m_key_set_playing_screenset));
    controltable->attach(*label, 6, 7, 2, 3);
    controltable->attach(*entry, 7, 8, 2, 3);


    /*Frame for sequence toggle keys*/
    Frame* toggleframe = manage(new Frame("Sequence toggle keys"));
    toggleframe->set_border_width(4);
    mainbox->pack_start(*toggleframe, Gtk::PACK_SHRINK);

    Table* toggletable = manage(new Table(4, 16, false));
    toggletable->set_border_width(4);
    toggletable->set_spacings(4);
    toggleframe->add(*toggletable);

    int x = 0;
    int y = 0;
    Label* numlabel;

    for (int i = 0; i < 32; i++) {
        x = i % 8 * 2;
        y = i / 8;
        int slot = x * 2 + y; // count this way: 0, 4, 8, 16...
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", slot);
        numlabel = manage(new Label(buf, Gtk::ALIGN_RIGHT));
        entry = manage(new KeyBindEntry(KeyBindEntry::events, NULL,
                    m_perf, slot));
        toggletable->attach(*numlabel, x, x + 1, y, y + 1);
        toggletable->attach(*entry, x + 1, x + 2, y, y + 1);
    }

    /*Frame for mute group slots*/
    Frame* mutegroupframe = manage(new Frame("Mute-group slots"));
    mutegroupframe->set_border_width(4);
    mainbox->pack_start(*mutegroupframe, Gtk::PACK_SHRINK);

    Table* mutegrouptable = manage(new Table(4, 16, false));
    mutegrouptable->set_border_width(4);
    mutegrouptable->set_spacings(4);
    mutegroupframe->add(*mutegrouptable);

    for (int i = 0; i <32; i++) {
        x = i%8*2;
        y = i/8;
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", i);
        numlabel = manage(new Label(buf, Gtk::ALIGN_RIGHT));
        entry = manage(new KeyBindEntry(KeyBindEntry::groups, NULL,
                    m_perf, i));
        mutegrouptable->attach(*numlabel, x, x + 1, y, y + 1);
        mutegrouptable->attach(*entry, x + 1, x + 2, y, y + 1);
    }


    #define AddKey(text, integer) \
    label = manage(new Label(text)); \
    hbox->pack_start(*label, false, false, 4); \
    entry = manage(new KeyBindEntry(KeyBindEntry::location, &integer)); \
    hbox->pack_start(*entry, false, false, 4);

    hbox = manage(new HBox());
    AddKey("Learn (while pressing a mute-group key):",
            m_perf->m_key_group_learn);
    AddKey("Disable:", m_perf->m_key_group_off);
    AddKey("Enable:", m_perf->m_key_group_on);
    mainbox->pack_start (*hbox, false, false);

#undef AddKey
}


/*Mouse page*/
void
options::add_mouse_page()
{
    VBox *vbox = manage(new VBox());
    m_notebook->append_page(*vbox, "_Mouse", true);

    /*Frame for transport options*/
    Frame* interactionframe = manage(new Frame("Interaction method"));
    interactionframe->set_border_width(4);
    vbox->pack_start(*interactionframe, Gtk::PACK_SHRINK);

    VBox *interactionbox = manage(new VBox());
    interactionbox->set_border_width(4);
    interactionframe->add(*interactionbox);

    Gtk::RadioButton *rb_seq24 = manage(new RadioButton(
                "se_q24 (original style)", true));
    interactionbox->pack_start(*rb_seq24, Gtk::PACK_SHRINK);

    Gtk::RadioButton * rb_fruity = manage(new RadioButton(
                "_fruity (similar to a certain well known sequencer)", true));
    interactionbox->pack_start(*rb_fruity, Gtk::PACK_SHRINK);

    Gtk::RadioButton::Group group = rb_seq24->get_group();
    rb_fruity->set_group(group);

    switch(global_interactionmethod)
    {
        case e_fruity_interaction:
            rb_fruity->set_active();
            break;

        case e_seq24_interaction:
        default:
            rb_seq24->set_active();
            break;
    }

    rb_seq24->signal_toggled().connect(sigc::bind(mem_fun(*this,
                    &options::mouse_seq24_callback), rb_seq24));

    rb_fruity->signal_toggled().connect(sigc::bind(mem_fun(*this,
                    &options::mouse_fruity_callback), rb_fruity));
}


/*Jack Sync page */
void
options::add_jack_sync_page()
{
#ifdef JACK_SUPPORT
    VBox *vbox = manage(new VBox());
    vbox->set_border_width(4);
    m_notebook->append_page(*vbox, "_Jack Sync", true);

    /*Frame for transport options*/
    Frame* transportframe = manage(new Frame("Transport"));
    transportframe->set_border_width(4);
    vbox->pack_start(*transportframe, Gtk::PACK_SHRINK);

    VBox *transportbox = manage(new VBox());
    transportbox->set_border_width(4);
    transportframe->add(*transportbox);

    CheckButton *check = manage(new CheckButton("Jack _Transport", true));
    check->set_active (global_with_jack_transport);
    add_tooltip( check, "Enable sync with JACK Transport.");
    check->signal_toggled().connect(bind(mem_fun(*this,
                    &options::transport_callback), e_jack_transport, check));
    transportbox->pack_start(*check, false, false);

    check = manage(new CheckButton("Trans_port Master", true));
    check->set_active (global_with_jack_master);
    add_tooltip( check, "Seq24 will attempt to serve as JACK Master.");
    check->signal_toggled().connect(bind(mem_fun(*this,
                    &options::transport_callback), e_jack_master, check));

    transportbox->pack_start(*check, false, false);

    check = manage (new CheckButton ("Master C_onditional", true));
    check->set_active (global_with_jack_master_cond);
    add_tooltip( check,
            "Seq24 will fail to be master if there is already a master set.");
    check->signal_toggled().connect(bind(mem_fun(*this,
                    &options::transport_callback), e_jack_master_cond, check));

    transportbox->pack_start(*check, false, false);


    /*Frame for jack start mode options*/
    Frame* modeframe = manage(new Frame("Jack start mode"));
    modeframe->set_border_width(4);
    vbox->pack_start(*modeframe, Gtk::PACK_SHRINK);

    VBox *modebox = manage(new VBox());
    modebox->set_border_width(4);
    modeframe->add(*modebox);

    Gtk::RadioButton * rb_live = manage(
            new RadioButton("_Live Mode", true));
    add_tooltip(rb_live, "Playback will be in live mode.  Use this to "
            "allow muting and unmuting of loops.");

    Gtk::RadioButton * rb_perform = manage(
            new RadioButton("_Song Mode", true));
    add_tooltip( rb_perform, "Playback will use the song editors data.");

    Gtk::RadioButton::Group group = rb_live->get_group ();
    rb_perform->set_group (group);

    if (global_jack_start_mode)
        rb_perform->set_active(true);
    else
        rb_live->set_active(true);

    rb_perform->signal_toggled().connect(bind(mem_fun(*this,
                    &options::transport_callback), e_jack_start_mode_song,
                rb_perform));

    modebox->pack_start(*rb_live, false, false);
    modebox->pack_start(*rb_perform, false, false);


    /*Connetion buttons*/
    HButtonBox* buttonbox = manage(new HButtonBox());
    buttonbox->set_layout(Gtk::BUTTONBOX_START);
    buttonbox->set_spacing(6);
    vbox->pack_start(*buttonbox, false, false);

    Gtk::Button * button = manage(new Button("Co_nnect", true));
    add_tooltip(button, "Connect to Jack.");
    button->signal_clicked().connect(bind(mem_fun(*this,
                    &options::transport_callback), e_jack_connect, button));
    buttonbox->pack_start(*button, false, false);

    button = manage(new Button("_Disconnect", true));
    add_tooltip(button, "Disconnect Jack.");
    button->signal_clicked().connect(bind(mem_fun(*this,
                    &options::transport_callback), e_jack_disconnect, button));
    buttonbox->pack_start(*button, false, false);
#endif
}

void
options::clock_callback_off (int a_bus, RadioButton *a_button)
{
    if (a_button->get_active ())
        m_perf->get_master_midi_bus ()->set_clock(a_bus, e_clock_off );
}

void
options::clock_callback_on (int a_bus, RadioButton *a_button)
{
    if (a_button->get_active ())
        m_perf->get_master_midi_bus ()->set_clock(a_bus, e_clock_pos );
}

void
options::clock_callback_mod (int a_bus, RadioButton *a_button)
{
    if (a_button->get_active ())
        m_perf->get_master_midi_bus ()->set_clock(a_bus, e_clock_mod );
}

void
options::clock_mod_callback( Adjustment *adj )
{
    midibus::set_clock_mod((int)adj->get_value());
}


void
options::input_callback (int a_bus, Button * i_button)
{
    CheckButton *a_button = (CheckButton *) i_button;
    bool input = a_button->get_active ();
    if (9999 == a_bus) {
        m_perf->m_show_ui_sequence_key = input;
        for (int i=0; i< c_max_sequence; i++ ){
            if (m_perf->get_sequence( i ))
                m_perf->get_sequence( i )->set_dirty();
        }
        return;
    }
    m_perf->get_master_midi_bus ()->set_input (a_bus, input);
}


void
options::mouse_seq24_callback(Gtk::RadioButton *btn)
{
    if (btn->get_active())
        global_interactionmethod = e_seq24_interaction;
}

void
options::mouse_fruity_callback(Gtk::RadioButton *btn)
{
    if (btn->get_active())
        global_interactionmethod = e_fruity_interaction;
}

void
options::transport_callback(button a_type, Button *a_check)
{
    CheckButton *check = (CheckButton *) a_check;

    switch (a_type)
    {
        case e_jack_transport:
            global_with_jack_transport = check->get_active();
            break;

        case e_jack_master:
            global_with_jack_master = check->get_active();
            break;

        case e_jack_master_cond:
            global_with_jack_master_cond = check->get_active();
            break;

        case e_jack_start_mode_song:
            global_jack_start_mode = check->get_active();
            break;

        case e_jack_connect:
            m_perf->init_jack();
            break;

        case e_jack_disconnect:
            m_perf->deinit_jack();
            break;

        default:
            break;
    }
}
