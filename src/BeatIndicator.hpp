#ifndef BEATINDICATOR_HPP
#define BEATINDICATOR_HPP

#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "MidiPerformance.hpp"

//A beat indicator widget.

class BeatIndicator : public QWidget
{
    Q_OBJECT

public:
    BeatIndicator(QWidget *parent,
                  MidiPerformance *perf,
                  int beats_per_measure,
                  int beat_width);

    ~BeatIndicator();

    QSize sizeHint() const;
    
    int getBeatsPerMeasure() const;
    void setBeatsPerMeasure(int beats_per_measure);
    
    int getBeatWidth() const;
    void setBeatWidth(int beat_width);
    
protected:
    //override painting event to draw on the frame
    void paintEvent(QPaintEvent *event);

private:
    MidiPerformance     * const m_main_perf;

    QPainter    *m_painter;

    int         m_beats_per_measure;
    int         m_beat_width;

};

#endif // BEATINDICATOR_HPP
