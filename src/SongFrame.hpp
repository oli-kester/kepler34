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

private:
    /* set snap to in pulses */
    int m_snap;

    int m_bpm;
    int m_bw;

    void set_bpm( int a_beats_per_measure );
    void set_bw( int a_beat_width );
    void set_snap (int a_snap);

    void set_guides();

    void grow ();

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
    void updateGridSnap(int snap);
};

#endif // SONGFRAME_HPP
