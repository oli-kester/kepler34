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
    alpha = 255;

    //start refresh timer to queue regular redraws
    mRedrawTimer = new QTimer(this);
    mRedrawTimer->setInterval(50);
    connect(mRedrawTimer,
            SIGNAL(timeout()),
            this,
            SLOT(update()));
    mRedrawTimer->start();

}

void BeatIndicator::paintEvent(QPaintEvent *)
{
    mPainter    = new QPainter(this);
    mBrush      = new QBrush(Qt::SolidPattern);
    mPen        = new QPen(Qt::darkGray);

    long tick = m_main_perf->get_tick();
    int metro = (tick / (c_ppqn / 4 * beatWidth)) % beatsPerMeasure;
    int divX = width() / beatsPerMeasure;
    int offsetX = divX * metro;

    //flash on beats
    if (metro != lastMetro)
    {
        alpha = 255;
        if (metro == 0)
        {
            //red on first in bar
            mColour->setRgb(255, 0, 0);
        }
        else
        {
            mColour->setRgb(255, 255, 255);
        }
    }

    mColour->setAlpha(alpha);
    mBrush->setColor(*mColour);
    mPainter->setPen(*mPen);
    mPainter->setFont(mFont);
    mPainter->setBrush(*mBrush);

    //draw background
    mPainter->drawRect(0, 0, width() - 1,height() - 1);

    //draw beat number
    mPen->setColor(Qt::black);
    mPainter->setPen(*mPen);
    mPainter->drawText(offsetX + divX * 0.5 - mFont.pointSize() * 0.5,
                       height() * 0.8,
                       QString::number(metro + 1));


    //lessen alpha on each redraw to have smooth fading
    alpha *= 0.7;

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
    return QSize(100, mFont.pointSize() * 2);
}
