#ifndef EDITNOTEROLL_HPP
#define EDITNOTEROLL_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QMouseEvent>

///
/// \brief The EditNoteRoll class
///
/// The MIDI note grid in the sequence editor
///

class EditNoteRoll : public QWidget
{
    Q_OBJECT

public:
    explicit EditNoteRoll(MidiPerformance *a_perf,
                          MidiSequence *a_seq,
                          QWidget *parent = 0);

    void set_snap(int snap);

    int length() const;
    void setNote_length(int length);

protected:
    //override painting event to draw on the frame
    void paintEvent         (QPaintEvent *);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

    //override keyboard events for interaction
    void keyPressEvent      (QKeyEvent * event);
    void keyReleaseEvent    (QKeyEvent * event);

    //override the sizehint to set our own defaults
    QSize sizeHint() const;

private:
    //performs a 'snap' on y
    void snap_y( int *a_y );

    //performs a 'snap' on x
    void snap_x( int *a_x );

    /* takes screen corrdinates, give us notes and ticks */
    void convert_xy( int a_x, int a_y, long *a_ticks, int *a_note);

    void set_adding(bool a_adding);

    MidiPerformance *m_perform;
    MidiSequence    *m_seq;

    QPen        *m_pen;
    QBrush      *m_brush;
    QPainter    *m_painter;
    QFont        m_font;
    QTimer      *m_timer;

    int m_scale;
    int m_key;
    int m_zoom;
    int m_snap;

    int m_note_length;

    /* when highlighting a bunch of events */
    bool m_selecting;
    bool m_adding;
    bool m_moving;
    bool m_moving_init;
    bool m_growing;
    bool m_painting;
    bool m_paste;
    bool m_is_drag_pasting;
    bool m_is_drag_pasting_start;
    bool m_justselected_one;

    //mouse tracking
    int m_drop_x;
    int m_drop_y;
    int m_move_delta_x;
    int m_move_delta_y;
    int m_current_x;
    int m_current_y;
    int m_move_snap_offset_x;

    //playhead tracking
    int m_old_progress_x;

    //background sequence
    int m_background_sequence;
    bool m_drawing_background_seq;

signals:

public slots:

};

#endif // EDITNOTEROLL_HPP
