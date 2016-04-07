#ifndef EDITTIMEBAR_HPP
#define EDITTIMEBAR_HPP

#include "MidiSequence.hpp"

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPen>

///
/// \brief The EditTimeBar class
///
/// The timebar for the sequence editor

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
    QBrush      *m_brush;
    QPainter    *m_painter;
    QFont        m_font;

    int m_zoom;

};

#endif // EDITTIMEBAR_HPP
