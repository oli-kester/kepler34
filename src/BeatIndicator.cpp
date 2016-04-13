#include "BeatIndicator.hpp"

BeatIndicator::BeatIndicator(QWidget *parent,
                             MidiPerformance *perf,
                             int beats_per_measure,
                             int beat_width):
    QWidget(parent),
    m_main_perf(perf),
    mBeatsPerMeasure(beats_per_measure),
    mBeatWidth(beat_width)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);
}

void BeatIndicator::paintEvent(QPaintEvent *)
{
    long tick = m_main_perf->get_tick();

    mPainter = new QPainter(this);
    mBrush = new QBrush(this);

    mPainter->setPen(Qt::black);

    long metro = (tick / (c_ppqn / 4 * mBeatWidth)) % mBeatsPerMeasure+ 1;

    mPainter->drawText(5, 20, QString::number(metro));

//    qDebug() << "BeatIndicator, paintEvent, Current tick: " << tick << endl;

    //draw background
    mPainter->drawRect(0,0,width()-1,height()-1);

    delete mPainter,mBrush;
}

int BeatIndicator::getBeatWidth() const
{
    return mBeatWidth;
}

void BeatIndicator::setBeatWidth(int beat_width)
{
    mBeatWidth = beat_width;
}

int BeatIndicator::getBeatsPerMeasure() const
{
    return mBeatsPerMeasure;
}

void BeatIndicator::setBeatsPerMeasure(int beats_per_measure)
{
    mBeatsPerMeasure = beats_per_measure;
}

BeatIndicator::~BeatIndicator()
{

}

QSize BeatIndicator::sizeHint() const
{
    return QSize(300, 50);
}
