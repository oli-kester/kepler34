/* main window */

#include "mainwid.h"
#include "perform.h"
#include "sequence.h"
#include "event.h"
#include "maintime.h"
#include "perfedit.h"
#include "options.h"

#pragma once

#include <map>
#include <gtkmm.h>
#include <string>

#include "globals.h"

using namespace Gtk;


using namespace Menu_Helpers;


class mainwnd : public Gtk::Window, public performcallback
{
    /* notification handler for learn mode toggle */
    virtual void on_grouplearnchange(bool state);

 private:

    perform  *m_mainperf;
    bool      m_modified;
    static int m_sigpipe[2];

#if GTK_MINOR_VERSION < 12
    Tooltips *m_tooltips;
#endif

    /* window elements */
    MenuBar  *m_menubar;
    Menu     *m_menu_file;
    Menu     *m_menu_file_recent;
    Menu     *m_menu_view;
    Menu     *m_menu_help;

    VBox     *mainvbox;
    VBox     *contentvbox;

    mainwid  *m_main_wid;
    maintime *m_main_time;

    perfedit *m_perf_edit;
    options *m_options;

    Gdk::Cursor   m_main_cursor;

    Button      *m_button_learn;

    ToggleButton *m_button_song_playback;

    Button      *m_button_stop;
    Button      *m_button_play;
    Button      *m_button_perfedit;

    SpinButton  *m_spinbutton_bpm;
    Adjustment  *m_adjust_bpm;

    SpinButton  *m_spinbutton_ss;
    Adjustment  *m_adjust_ss;

    SpinButton  *m_spinbutton_load_offset;
    Adjustment  *m_adjust_load_offset;

    Entry       *m_entry_notes;

    sigc::connection   m_timeout_connect;

    /* file dialogs */
    void file_import_dialog();
    void options_dialog();
    void about_dialog();

    /* callback methods */
    void adj_callback_ss( );
    void adj_callback_bpm( );
    void edit_callback_notepad( );
    bool timer_callback( );

    /* playback */
    void start_playing();
    void stop_playing();
    void learn_toggle();
    void song_playback_toggle();
    void open_performance_edit( );
    void sequence_key( int a_seq );
    void update_window_title();
    void toLower(basic_string<char>&);

    /* file management */
    bool is_modified();
    void file_new();
    void file_open();
    void file_save();
    void file_save_as();
    void file_exit();
    void new_file();
    bool save_file();
    void choose_file();
    int query_save_changes();
    bool is_save();

    /* signal handlers */
    static void handle_signal(int sig);
    bool install_signal_handlers();
    bool signal_action(Glib::IOCondition condition);

    /* repopulate the menu bar and redraw the main window */
    void redraw_menu();

    /* hook functions for loading recent files */
    void load_recent_1();
    void load_recent_2();
    void load_recent_3();
    void load_recent_4();
    void load_recent_5();
    void load_recent_6();
    void load_recent_7();
    void load_recent_8();
    void load_recent_9();
    void load_recent_10();

 public:

    mainwnd(perform *a_p);
    ~mainwnd();

    void open_file(const Glib::ustring&);

    bool on_delete_event(GdkEventAny *a_e);
    bool on_key_press_event(GdkEventKey* a_ev);
    bool on_key_release_event(GdkEventKey* a_ev);
};
