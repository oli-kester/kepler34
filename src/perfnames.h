/* draws and handles the loop labels
 * on the left of the song editor window */

#pragma once

#include "perform.h"
#include "sequence.h"
#include "seqmenu.h"

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
#include <gtkmm/adjustment.h>

using namespace Gtk;

#include "globals.h"

/* holds the left side piano */
class perfnames : public virtual Gtk::DrawingArea, public virtual seqmenu
{
 private:

    Glib::RefPtr<Gdk::GC>       m_gc;
    Glib::RefPtr<Gdk::Window>   m_window;
    Gdk::Color    m_black, m_white, m_grey;

    Glib::RefPtr<Gdk::Pixmap>   m_pixmap;

    perform      *m_mainperf;

    Adjustment   *m_vadjust;

    int m_window_x, m_window_y;

    int          m_sequence_offset;

    bool         m_sequence_active[c_total_seqs];

    void on_realize();
    bool on_expose_event(GdkEventExpose* a_ev);
    bool on_button_press_event(GdkEventButton* a_ev);
    bool on_button_release_event(GdkEventButton* a_ev);
    void on_size_allocate(Gtk::Allocation& );
    bool on_scroll_event( GdkEventScroll* a_ev ) ;

    void draw_area();
    void update_pixmap();

    void convert_y( int a_y, int *a_note);

    void draw_sequence( int a_sequence );

    void change_vert();

    void redraw( int a_sequence );

 public:

    void redraw_dirty_sequences();

    perfnames( perform *a_perf,
	       Adjustment *a_vadjust   );
};
