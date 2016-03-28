#ifndef BEATINDICATOR_HPP
#define BEATINDICATOR_HPP

#include <QWidget>

#include "MidiPerformance.hpp"

class BeatIndicator : public QWidget
{
    Q_OBJECT

public:
    BeatIndicator(MidiPerformance *perf, int beats_per_measure, int beat_width);

private:
    MidiPerformance     * const m_main_perf;

    int     m_beats_per_measure;
    int     m_beat_width;

};

#endif // BEATINDICATOR_HPP
