#ifndef SONGTIMEBAR_HPP
#define SONGTIMEBAR_HPP

#include "Globals.hpp"
#include "MidiPerformance.hpp"

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QObject>
#include <QPen>
#include <QMouseEvent>

///
/// \brief The SongTimeBar class
///
/// The time bar for the song editor

class SongTimeBar : public QWidget
{
    Q_OBJECT
public:
    explicit SongTimeBar(MidiPerformance *a_perf,
                         QWidget *parent);

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

private:
    MidiPerformance *m_mainperf;

    QTimer      *mTimer;
    QPen        *mPen;
    QBrush      *mBrush;
    QPainter    *mPainter;
    QFont        mFont;

    int m_4bar_offset;

    int m_snap, m_measure_length;

    int zoom;

signals:

public slots:
};

#endif // SONGTIMEBAR_HPP
