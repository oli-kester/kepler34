#ifndef LIVEFRAME_HPP
#define LIVEFRAME_HPP

#include <QFrame>

namespace Ui {
class LiveFrame;
}

class LiveFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LiveFrame(QWidget *parent = 0);
    ~LiveFrame();

private:
    Ui::LiveFrame *ui;
};

#endif // LIVEFRAME_HPP
