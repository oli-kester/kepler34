#ifndef EDITTIMEBAR_HPP
#define EDITTIMEBAR_HPP

#include "MidiSequence.hpp"

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPen>

class EditTimeBar : public QWidget
{
    Q_OBJECT

public:
    explicit EditTimeBar(MidiSequence *a_seq,
                         QWidget *parent);

protected:
    //override painting event to draw on the frame
    void paintEvent         (QPaintEvent *);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

    //override the sizehint to set our own defaults
    QSize sizeHint() const;

signals:

private slots:

private:
    MidiSequence *m_seq;

    QTimer      *m_timer;
    QPen        *m_pen;
    QPainter    *m_painter;
    QRect        m_rect;
    QPoint       m_point;
    QFont        m_font;
    QLine        m_line;

    int m_zoom;
    int m_scroll_offset_ticks;
    int m_scroll_offset_x;
    int m_window_x, m_window_y;

};

#endif // EDITTIMEBAR_HPP
