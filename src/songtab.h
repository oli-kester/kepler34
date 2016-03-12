#pragma once

#include "sequence.h"
#include "perform.h"

#include <gtkmm/adjustment.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/box.h>
#include <gtkmm/main.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/window.h>
#include <gtkmm/table.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/widget.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/viewport.h>
#include <gtkmm/combo.h>
#include <gtkmm/label.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/optionmenu.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/invisible.h>
#include <gtkmm/separator.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/invisible.h>
#include <gtkmm/arrow.h>
#include <gtkmm/image.h>

#include <sigc++/bind.h>

#include <list>
#include <string>

#include "globals.h"
#include "mainwid.h"
#include "perfnames.h"
#include "perfroll.h"
#include "perftime.h"


using namespace Gtk;

/* piano roll */
class songtab : public Gtk::Table
{
private:

  perform * m_mainperf;
  perfnames *m_perfnames;
  perfroll *m_perfroll;
  perftime *m_perftime;

  /* GUI widgets */
  Table *m_table;

  VScrollbar *m_vscroll;
  HScrollbar *m_hscroll;

  Adjustment *m_vadjust;
  Adjustment *m_hadjust;

  Menu *m_menu_snap;
  Button *m_button_snap;
  Entry *m_entry_snap;

  Button *m_button_stop;
  Button *m_button_play;
  ToggleButton *m_button_record;
  ToggleButton *m_button_loop;

  Button *m_button_expand;
  Button *m_button_collapse;
  Button *m_button_copy;

  Button *m_button_grow;
  Button *m_button_undo;
  ToggleButton *m_button_song_playback;

  Button      *m_button_bpm;
  Entry       *m_entry_bpm;

  Button      *m_button_bw;
  Entry       *m_entry_bw;

  HBox *m_hbox;
  HBox *m_hlbox;

  Tooltips *m_tooltips;

  /* time signature, beats per measure, beat width */
  Menu       *m_menu_bpm;
  Menu       *m_menu_bw;

  /* set snap to in pulses */
  int m_snap;

  int m_bpm;
  int m_bw;

  void set_bpm( int a_beats_per_measure );
  void set_bw( int a_beat_width );
  void set_snap (int a_snap);

  void set_guides();

  void grow ();

  void on_realize ();

  /* playback methods */
  void start_playing ();
  void stop_playing ();
  void set_looped ();
  void record_toggle ();

  void expand ();
  void collapse ();

  void copy ();
  void undo ();

  /* right click context menu for loop options */
  void popup_menu (Menu * a_menu);

  bool timeout ();

  bool on_delete_event (GdkEventAny * a_event);
  bool on_key_press_event(GdkEventKey* a_ev);

public:

  void init_before_show ();

  songtab (perform * a_perf);
  ~songtab ();

};

