#include "BeatIndicator.hpp"

BeatIndicator::BeatIndicator(MidiPerformance *perf, int beats_per_measure, int beat_width):
    m_main_perf(perf),
    m_beats_per_measure(beats_per_measure),
    m_beat_width(beat_width)
{

}
