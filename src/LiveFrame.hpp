#ifndef LIVEFRAME_HPP
#define LIVEFRAME_HPP

#include "MidiPerformance.hpp"

#include <QFrame>
#include <QPainter>

namespace Ui {
class LiveFrame;
}

class LiveFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LiveFrame(QWidget *parent = 0, MidiPerformance *perf = 0);
    ~LiveFrame();

protected:
    //override painting event to draw on the frame
    void paintEvent(QPaintEvent *event);

private:
    //draw a single sequence thumbnail at index
    void drawSequence(int a_seq);

    //draw all sequences
    void drawAllSequences();

    Ui::LiveFrame *ui;

    MidiPerformance *m_main_perf;

    QPainter    *m_painter;
    QPen        *m_background;
    QPen        *m_foreground;
    QRect       *rect;

    int     m_screenset;
    long    m_last_tick_x[c_max_sequence];
    bool    m_last_playing[c_max_sequence];

};

#endif // LIVEFRAME_HPP
