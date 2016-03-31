#ifndef LIVEFRAME_HPP
#define LIVEFRAME_HPP

#include "MidiPerformance.hpp"

#include <QFrame>
#include <QPainter>
#include <QDebug>

namespace Ui {
class LiveFrame;
}

class LiveFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LiveFrame(QWidget *parent = 0, MidiPerformance *perf = 0);
    ~LiveFrame();

    void setBank(int newBank);

protected:
    //override painting event to draw on the frame
    void paintEvent(QPaintEvent *event);
    //redraw frame contents
    void redraw();

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

    int     m_bank;
    long    m_last_tick_x[c_max_sequence];
    bool    m_last_playing[c_max_sequence];

private slots:
    void updateBank(int newBank);
    void updateBankName();
};

#endif // LIVEFRAME_HPP
