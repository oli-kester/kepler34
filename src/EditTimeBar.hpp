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
    void paintEvent         (QPaintEvent *event);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

signals:

private slots:
    void refresh();

private:
    MidiSequence *m_seq;

    QTimer  *m_timer;
};

#endif // EDITTIMEBAR_HPP
