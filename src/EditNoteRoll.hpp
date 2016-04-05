#ifndef EDITNOTEROLL_HPP
#define EDITNOTEROLL_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"

#include <QWidget>
#include <QPainter>
#include <QPen>

class EditNoteRoll : public QWidget
{
    Q_OBJECT

public:
    explicit EditNoteRoll(MidiPerformance *a_perf,
                          MidiSequence *a_seq,
                          QWidget *parent = 0);

protected:
    //override painting event to draw on the frame
    void paintEvent         (QPaintEvent *);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

    //override the sizehint to set our own defaults
    QSize sizeHint() const;

private:
    MidiPerformance *m_perform;
    MidiSequence    *m_seq;

    QPen        *m_pen;
    QPainter    *m_painter;
    QRect        m_rect;
    QPoint       m_point;
    QFont        m_font;
    QLine        m_line;

    int m_window_x, m_window_y;

    int m_scroll_offset_ticks;
    int m_scroll_offset_key;
    int m_scroll_offset_x;
    int m_scroll_offset_y;

    int m_scale;
    int m_key;
    int m_zoom;
    int m_snap;

    int m_old_progress_x;

    int m_background_sequence;
    bool m_drawing_background_seq;

signals:

public slots:

};

#endif // EDITNOTEROLL_HPP
