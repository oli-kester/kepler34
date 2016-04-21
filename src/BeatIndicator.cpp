#include "BeatIndicator.hpp"

BeatIndicator::BeatIndicator(QWidget *parent,
                             MidiPerformance *perf,
                             int beats_per_measure,
                             int beat_width):
    QWidget(parent),
    m_main_perf(perf),
    beatsPerMeasure(beats_per_measure),
    beatWidth(beat_width)
{
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::Fixed);

    mFont.setPointSize(9);
    mFont.setBold(true);

    mColour     = new QColor(Qt::red);
    alpha = 0;
    lastMetro = 0;
}
void BeatIndicator::paintEvent(QPaintEvent *)
{
    mPainter    = new QPainter(this);
    mBrush      = new QBrush(Qt::NoBrush);
    mPen        = new QPen(Qt::darkGray);

    mPainter->setPen(*mPen);
    mPainter->setFont(mFont);
    mPainter->setBrush(*mBrush);

    long tick = m_main_perf->get_tick();
    int metro = (tick / (c_ppqn / 4 * beatWidth)) % beatsPerMeasure;
    int divX = (width() - 1) / beatsPerMeasure;

    //flash on beats
    //(i.e. if metro has changed or we've just started playing)
    if (metro != lastMetro || (tick < 50 && tick > 0 ))
    {
        alpha = 230;
        if (metro == 0)
        {
            //red on first beat in bar
            mColour->setRgb(255, 50, 50);
        }
        else
        {
            //white on others
            mColour->setRgb(255, 255, 255);
        }
    }

    //draw beat blocks
    for (int i = 0; i < beatsPerMeasure; i++)
    {
        int offsetX = divX * i;

        //with flash if on current beat
        if (i == metro && m_main_perf->is_running())
        {
            mBrush->setStyle(Qt::SolidPattern);
            mPen->setColor(Qt::black);
        }
        else
        {
            mBrush->setStyle(Qt::NoBrush);
            mPen->setColor(Qt::darkGray);

        }

        mColour->setAlpha(alpha);
        mBrush->setColor(*mColour);
        mPainter->setPen(*mPen);
        mPainter->setBrush(*mBrush);
        mPainter->drawRect(offsetX + mPen->width() - 1,
                           mPen->width() - 1,
                           divX - mPen->width(),
                           height() - mPen->width());
    }

    //draw beat number (if there's space)
    if (beatsPerMeasure < 10)
    {
        mPen->setColor(Qt::black);
        mPen->setStyle(Qt::SolidLine);
        mPainter->setPen(*mPen);
        mPainter->drawText((metro + 1) * divX - (mFont.pointSize() + 2),
                           height() * 0.3 + mFont.pointSize(),
                           QString::number(metro + 1));
    }

    //lessen alpha on each redraw to have smooth fading
    //done as a factor of the bpm to get useful fades
    alpha *= 0.7 - m_main_perf->get_bpm() / 300;

    lastMetro = metro;

    delete mPainter, mBrush, mPen;
}

int BeatIndicator::getBeatWidth() const
{
    return beatWidth;
}

void BeatIndicator::setBeatWidth(int beat_width)
{
    beatWidth = beat_width;
}

int BeatIndicator::getBeatsPerMeasure() const
{
    return beatsPerMeasure;
}

void BeatIndicator::setBeatsPerMeasure(int beats_per_measure)
{
    beatsPerMeasure = beats_per_measure;
}

BeatIndicator::~BeatIndicator()
{

}

QSize BeatIndicator::sizeHint() const
{
    return QSize(150, mFont.pointSize() * 2.4);
}
