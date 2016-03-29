#include "BeatIndicator.hpp"

BeatIndicator::BeatIndicator(QWidget *parent, MidiPerformance *perf, int beats_per_measure, int beat_width):
    QWidget(parent),
    m_main_perf(perf),
    m_beats_per_measure(beats_per_measure),
    m_beat_width(beat_width)
{
    m_painter = new QPainter(this);

    m_painter->drawText(0, 0, "text");
}

QSize BeatIndicator::sizeHint() const
{
    return QSize(500, 500);
}

void BeatIndicator::paintEvent(QPaintEvent *)
{
    long tick = m_main_perf->get_tick();

    qDebug() << "BeatIndicator.cpp - paintEvent, Current tick: " << tick << endl;
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

BeatIndicator::~BeatIndicator()
{

}
