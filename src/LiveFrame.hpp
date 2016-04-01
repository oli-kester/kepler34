#ifndef LIVEFRAME_HPP
#define LIVEFRAME_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"

#include <QFrame>
#include <QPainter>
#include <QDebug>
#include <QMenu>

namespace Ui {
class LiveFrame;
}

class LiveFrame : public QFrame
{
    Q_OBJECT

public:

    explicit LiveFrame(QWidget *parent = 0, MidiPerformance *perf = 0);

    ~LiveFrame();

    //redraw frame contents
    void redraw();

    //set bank of sequences displayed
    void setBank(int newBank);



protected:

    //override painting event to draw on the frame
    void paintEvent(QPaintEvent *event);

    //override mouse events for interaction
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

private:

    //draw a single sequence thumbnail at index
    void drawSequence(int a_seq);

    //draw all sequences
    void drawAllSequences();

    //used to grab std::string bank name and
    //convert it to QString for display
    void banknameConvertDisplay();

    //converts the XY coordinates of a click into a seq ID
    int seqIDFromClickXY(int click_x, int click_y);

    Ui::LiveFrame *ui;

    MidiPerformance *m_main_perf;
    MidiSequence    m_moving_seq;

    QPainter    *m_painter;
    QPen        *m_background;
    QPen        *m_foreground;
    QRect       *rect;
    QMenu       *m_popup;

    int     m_bank_id;

    //mouse interaction
    int     m_current_seq;
    int     m_old_seq;
    bool    m_button_down;
    bool    m_moving;

    long    m_last_tick_x[c_max_sequence];
    bool    m_last_playing[c_max_sequence];


private slots:

    void updateBank(int newBank);
    void updateBankName();
    void newSeq();
    void editSeq();

signals:

    void callEditor(MidiSequence *seq);
};

#endif // LIVEFRAME_HPP
