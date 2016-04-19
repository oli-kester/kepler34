#ifndef EDITEVENTVALUES_HPP
#define EDITEVENTVALUES_HPP

#include "MidiSequence.hpp"

#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

///
/// \brief The EditEventValues class
///
/// Displays the values for MIDI events
/// e.g. Modwheel, pitchbend etc

class EditEventValues : public QWidget
{
    Q_OBJECT

public:
    explicit EditEventValues(MidiSequence *a_seq,
                             QWidget *parent = 0);

    void zoomIn();
    void zoomOut();
    void set_data_type(unsigned char a_status, unsigned char a_control);

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

    // Takes two points, returns a Xwin rectangle
    void xy_to_rect(int a_x1,  int a_y1,
                    int a_x2,  int a_y2,
                    int *a_x,  int *a_y,
                    int *a_w,  int *a_h );

    //convert the given X coordinate into a tick via a pointer
    void convert_x( int a_x, long *a_tick );

    MidiSequence *m_seq;

    QPen        *mPen;
    QBrush      *mBrush;
    QPainter    *mPainter;
    QFont        mFont;
    QRect       *mOld;
    QString      mNumbers;
    QTimer      *mTimer;

    int m_zoom;

    int mDropX, mDropY;
    int mCurrentX, mCurrentY;

    /* what is the data window currently editing ? */
    unsigned char m_status;
    unsigned char m_cc;

    //interaction states
    bool mLineAdjust; //dragging a new-level adjustment slope
    bool mRelativeAdjust; //relative adjusting notes by dragging

    friend class EditNoteRoll;
    friend class EditEventTriggers;
};

#endif // EDITEVENTVALUES_HPP
