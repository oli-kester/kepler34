#ifndef EDITFRAME_HPP
#define EDITFRAME_HPP

#include "MidiPerformance.hpp"
#include "MidiSequence.hpp"

#include <QFrame>
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

    MidiSequence    * const m_seq;
    MidiPerformance    * const m_perf;
};

#endif // EDITFRAME_HPP
