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
//
#ifndef SEQ24_PERFROLL_INPUT_H
#define SEQ24_PERFROLL_INPUT_H

#include <gtkmm/button.h>

class perfroll;

class AbstractPerfInput
{
    public:
        virtual ~AbstractPerfInput() { };

        virtual bool on_button_press_event(GdkEventButton* a_ev, perfroll& ths) = 0;
        virtual bool on_button_release_event(GdkEventButton* a_ev, perfroll& ths) = 0;
        virtual bool on_motion_notify_event(GdkEventMotion* a_ev, perfroll& ths) = 0;
};

class FruityPerfInput : public AbstractPerfInput
{
    public:
        FruityPerfInput() : m_adding_pressed( false ), m_current_x( 0 ),
        m_current_y( 0 )
    {}
        bool on_button_press_event(GdkEventButton* a_ev, perfroll& ths);
        bool on_button_release_event(GdkEventButton* a_ev, perfroll& ths);
        bool on_motion_notify_event(GdkEventMotion* a_ev, perfroll& ths);

    private:
        void updateMousePtr(perfroll& ths);
        void on_left_button_pressed(GdkEventButton* a_ev, perfroll& ths);
        void on_right_button_pressed(GdkEventButton* a_ev, perfroll& ths);
        bool m_adding_pressed;
        long m_current_x, m_current_y;
};

class Seq24PerfInput : public AbstractPerfInput
{
    public:
        Seq24PerfInput() : m_adding( false ), m_adding_pressed( false ) {}
        bool on_button_press_event(GdkEventButton* a_ev, perfroll& ths);
        bool on_button_release_event(GdkEventButton* a_ev, perfroll& ths);
        bool on_motion_notify_event(GdkEventMotion* a_ev, perfroll& ths);

    private:
        void set_adding( bool a_adding, perfroll& ths );
        bool m_adding;
        bool m_adding_pressed;
};

#endif

