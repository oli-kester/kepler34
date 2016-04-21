#ifndef SONGFRAME_HPP
#define SONGFRAME_HPP

#include "MidiPerformance.hpp"
#include "SongSequenceGrid.hpp"
#include "SongSequenceNames.hpp"
#include "SongTimeBar.hpp"

#include <QFrame>
#include <QGridLayout>
#include <QScrollArea>
#include <qmath.h>

namespace Ui {
class SongFrame;
}

///
/// \brief The SongFrame class
///
/// Holds the song editing elements

class SongFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SongFrame(MidiPerformance *a_perf,
                       QWidget *parent);
    ~SongFrame();

    int getBeatWidth() const;
    void setBeatLength( int a_beat_width );

    int getBeatsPerMeasure() const;
    void setBeatsPerMeasure( int a_beats_per_measure );

    //calls update geometry on elements to react to changes
    //in MIDI file sizes
    void updateSizes();

private:
    /* set snap to in pulses */
    int m_snap;

    int mBeatsPerMeasure;
    int mBeatWidth;

    void setSnap (int a_snap);

    void setGuides();

    void grow();

    Ui::SongFrame *ui;

    QGridLayout     *m_layout_grid;
    QScrollArea     *m_scroll_area;
    QWidget         *mContainer;
    QPalette        *m_palette;

    MidiPerformance   *m_mainperf;

    SongSequenceGrid  *m_perfroll;
    SongSequenceNames *m_perfnames;
    SongTimeBar       *m_perftime;

private slots:
    void updateGridSnap(int snapIndex);
    void zoomIn();
    void zoomOut();
    void markerCollapse();
    void markerExpand();
    void markerExpandCopy();
    void markerLoop(bool loop);
};

#endif // SONGFRAME_HPP
