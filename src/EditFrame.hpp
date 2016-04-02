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

namespace Ui {
class EditFrame;
}

class EditFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EditFrame(QWidget *parent, MidiPerformance *perf, MidiSequence *seq);
    ~EditFrame();

private:
    Ui::EditFrame *ui;

    QHBoxLayout     *m_lay1;

    MidiSequence       * const m_seq;
    MidiPerformance    * const m_perf;

    EditKeys     *m_seqkeys_wid;
    EditTimeBar  *m_seqtime_wid;
    EditNoteRoll *m_seqroll_wid;

};

#endif // EDITFRAME_HPP
