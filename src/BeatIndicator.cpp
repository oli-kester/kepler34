#include "BeatIndicator.hpp"

BeatIndicator::BeatIndicator(MidiPerformance *perf, int beats_per_measure, int beat_width):
    m_main_perf(perf),
    m_beats_per_measure(beats_per_measure),
    m_beat_width(beat_width)
{

}

void BeatIndicator::paintEvent(QPaintEvent *)
{

}

int BeatIndicator::getbeatWidth() const
{
    return m_beat_width;
}

void BeatIndicator::setbeatWidth(int beat_width)
{
    m_beat_width = beat_width;
}

int BeatIndicator::getBeatsPerMeasure() const
{
    return m_beats_per_measure;
}

void BeatIndicator::setBeatsPerMeasure(int beats_per_measure)
{
    m_beats_per_measure = beats_per_measure;
}

//BeatIndicator::~BeatIndicator()
//{

//}

