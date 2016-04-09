#ifndef SONGSEQUENCENAMES_HPP
#define SONGSEQUENCENAMES_HPP

#include <MidiPerformance.hpp>
#include <MidiSequence.hpp>
#include <Globals.hpp>

#include <QWidget>
#include <QPainter>
#include <QPen>

///
/// \brief The SongSequenceNames class
///
/// Sequence labels for the side of the song editor

class SongSequenceNames : public QWidget
{
    Q_OBJECT
public:
    explicit SongSequenceNames(MidiPerformance *a_perf,
                               QWidget *parent);
    ~SongSequenceNames();

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
    MidiPerformance *m_mainperf;

    QTimer      *mTimer;
    QPen        *mPen;
    QBrush      *mBrush;
    QPainter    *mPainter;
    QFont        mFont;

    bool         m_sequence_active[c_total_seqs];
};

#endif // SONGSEQUENCENAMES_HPP
