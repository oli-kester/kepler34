#ifndef EDITEVENTVALUES_HPP
#define EDITEVENTVALUES_HPP

#include "MidiSequence.hpp"

#include <QWidget>
#include <QPainter>
#include <QPen>

class EditEventValues : public QWidget
{
    Q_OBJECT
public:
    explicit EditEventValues(MidiSequence *a_seq,
                             QWidget *parent = 0);

    void zoomIn();
    void zoomOut();

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

public slots:

private:
    MidiSequence *m_seq;

    QPen        *m_pen;
    QBrush      *m_brush;
    QPainter    *m_painter;
    QFont        m_font;

    int m_zoom;

};

#endif // EDITEVENTVALUES_HPP