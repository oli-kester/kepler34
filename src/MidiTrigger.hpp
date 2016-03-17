/* class to handle triggers for notes and seq on/off */

#ifndef MIDITRIGGER_HPP
#define MIDITRIGGER_HPP

class MidiTrigger
{
public:

    long m_tick_start;
    long m_tick_end;

    bool m_selected;

    long m_offset;

    MidiTrigger()
    {
        m_tick_start = 0;
        m_tick_end = 0;
        m_offset = 0;
        m_selected = false;
    }

    bool operator< (MidiTrigger rhs){

        if (m_tick_start < rhs.m_tick_start)
            return true;

        return false;
    }
};

#endif // MIDITRIGGER_HPP
