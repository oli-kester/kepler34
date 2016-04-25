#ifndef EDITFRAME_HPP
#define EDITFRAME_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"
#include "EditKeys.hpp"
#include "EditTimeBar.hpp"
#include "EditNoteRoll.hpp"
#include "EditEventValues.hpp"
#include "EditEventTriggers.hpp"

#include <QFrame>
#include <QLayout>
#include <qmath.h>
#include <QScrollBar>
#include <QScrollArea>
#include <QPalette>
#include <QMenu>

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
    explicit EditFrame(QWidget *parent,
                       MidiPerformance *perf,
                       int mSeqId);
    ~EditFrame();

    void updateDrawGeometry();

    //set a new editing mode
    void setEditorMode(edit_mode_e mode);

private:
    Ui::EditFrame   *ui;

    QGridLayout     *m_layout_grid;
    QScrollArea     *m_scroll_area;
    QWidget         *mContainer;
    QPalette        *m_palette;
    QMenu           *mPopup;

    MidiSequence    * const mSeq;
    MidiPerformance * const mPerformance;

    EditKeys          *mKeyboard;
    EditTimeBar       *mTimeBar;
    EditNoteRoll      *mNoteGrid;
    EditEventValues   *mEventValues;
    EditEventTriggers *mEventTriggers;

    /* set snap to in pulses, off = 1 */
    int         mSnap;
    edit_mode_e editMode;
    int         mSeqId;

private slots:
    void updateSeqName();
    void updateGridSnap(int snapIndex);
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
    void toggleEditorMode();
    void updateRecVol();
    void toggleMidiPlay(bool newVal);
    void toggleMidiThru(bool newVal);
    void toggleMidiQRec(bool newVal);
    void toggleMidiRec(bool newVal);
    void selectAllNotes();
    void inverseNoteSelection();
    void quantizeNotes();
    void tightenNotes();
    void transposeNotes();
};

#endif // EDITFRAME_HPP
