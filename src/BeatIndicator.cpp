#include "BeatIndicator.hpp"

BeatIndicator::BeatIndicator(QWidget *parent,
                             MidiPerformance *perf,
                             int beats_per_measure,
                             int beat_width):
    QWidget(parent),
    m_main_perf(perf),
    m_beats_per_measure(beats_per_measure),
    m_beat_width(beat_width)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);
}

QSize BeatIndicator::sizeHint() const
{
    return QSize(30,20);
}

void BeatIndicator::paintEvent(QPaintEvent *)
{
    long tick = m_main_perf->get_tick();

    m_painter = new QPainter(this);

    m_painter->setPen(Qt::black);

    //beat indicator calculations from seq24
//    int tick_x = ((tick % c_ppqn) * (c_mainwid_x - 1) ) / c_ppqn;
//    int bar_x = (((tick / 16) % c_ppqn) * 4) / c_ppqn;

    //TODO refactor this equation
    int metro = ((((tick / getBeatsPerMeasure()) % c_ppqn) * getBeatWidth()) / c_ppqn) + 1;

    m_painter->drawText(5, 20, QString::number(metro));

//    qDebug() << "BeatIndicator, paintEvent, Current tick: " << tick << endl;
}

int BeatIndicator::getBeatWidth() const
{
    return m_beat_width;
}

void BeatIndicator::setBeatWidth(int beat_width)
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
