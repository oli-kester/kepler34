#ifndef EDITFRAME_HPP
#define EDITFRAME_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"
#include "EditKeys.hpp"
#include "EditTimeBar.hpp"
#include "EditNoteRoll.hpp"

#include <QFrame>
#include <QLayout>
#include <qmath.h>
#include <QScrollBar>
#include <QScrollArea>
#include <QPalette>

namespace Ui {
class EditFrame;
}

///
/// \brief The EditFrame class
///
/// Holds tools for editing an individual MIDI sequence

class EditFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EditFrame(QWidget *parent, MidiPerformance *perf, MidiSequence *seq);
    ~EditFrame();

private:
    long getMeasures();

    Ui::EditFrame   *ui;

    QGridLayout     *m_layout_grid;
    QScrollArea     *m_scroll_area;
    QScrollBar      *m_scroll_horiz;
    QScrollBar      *m_scroll_vert;
    QWidget         *m_container;
    QPalette        *m_palette;

    MidiSequence       * const m_seq;
    MidiPerformance    * const m_perf;

    EditKeys     *m_seqkeys_wid;
    EditTimeBar  *m_seqtime_wid;
    EditNoteRoll *m_seqroll_wid;

    /* set snap to in pulses, off = 1 */
    int         m_snap;

private slots:
    void updateSeqName();
    void updateGridSnap(int newSnap);
    void updateMidiBus(int newIndex);
    void updateMidiChannel(int newIndex);
    void undo();
    void redo();
    void showTools();
    void updateNoteLength(int newIndex);
    void zoomIn();
    void zoomOut();
    void updateKey(int newIndex);
    void updateSeqLength();
    void updateScale(int newIndex);
    void updateBackgroundSeq(int newIndex);
};

#endif // EDITFRAME_HPP
