#ifndef EDITNOTEROLL_HPP
#define EDITNOTEROLL_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>

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
    QBrush      *m_brush;
    QPainter    *m_painter;
    QRect        m_rect;
    QPoint       m_point;
    QFont        m_font;
    QLine        m_line;
    QTimer      *m_timer;

    int m_scale;
    int m_key;
    int m_zoom;
    int m_snap;

    int m_size_x;
    int m_size_y;

    int m_old_progress_x;

    int m_background_sequence;
    bool m_drawing_background_seq;

signals:

public slots:

};

#endif // EDITNOTEROLL_HPP
