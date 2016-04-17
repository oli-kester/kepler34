#ifndef EDITEVENTTRIGGERS_HPP
#define EDITEVENTTRIGGERS_HPP

#include "MidiSequence.hpp"
#include "EditEventValues.hpp"

#include <QWidget>
#include <QPainter>
#include <QPen>

class EditEventTriggers : public QWidget
{
    Q_OBJECT

public:
    explicit EditEventTriggers(MidiSequence *a_seq,
                               EditEventValues *a_seqdata_wid,
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
    /* checks mins / maxes..  the fills in x,y
       and width and height */
    void x_to_w( int a_x1, int a_x2,
         int *a_x, int *a_w  );

    MidiSequence    *m_seq;
    EditEventValues *m_seqdata_wid;

    QPen        *mPen;
    QBrush      *mBrush;
    QPainter    *mPainter;
    QFont        mFont;
    QRect       *m_old;
    QRect       *m_selected;

    /* one pixel == m_zoom ticks */
    int          m_zoom;
    int          m_snap;

    int m_window_x, m_window_y;

    /* when highlighting a bunch of events */
    bool m_selecting;
    bool m_moving_init;
    bool m_moving;
    bool m_growing;
    bool m_painting;
    bool m_paste;

    /* where the dragging started */
    int m_drop_x;
    int m_drop_y;
    int m_current_x;
    int m_current_y;

    int m_move_snap_offset_x;

    /* what is the data window currently editing ? */
    unsigned char m_status;
    unsigned char m_cc;
};

#endif // EDITEVENTTRIGGERS_HPP
