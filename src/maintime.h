/* beat indicator for the main window */

#pragma once

#include "globals.h"

const int c_maintime_x = 300;
const int c_maintime_y = 10;
const int c_pill_width = 8;

/* main time*/
class maintime: public Gtk::DrawingArea
{

 private:

    Glib::RefPtr<Gdk::GC>       m_gc;
    Glib::RefPtr<Gdk::Window>   m_window;
    Gdk::Color    m_black, m_white, m_grey;

    void on_realize();
    bool on_expose_event(GdkEventExpose* a_ev);

    long m_tick;

 public:

    int idle_progress( long a_ticks );
    maintime(  );

};

