#ifndef LIVEFRAME_HPP
#define LIVEFRAME_HPP

#include <QFrame>
#include <QPainter>

namespace Ui {
class LiveFrame;
}

class LiveFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LiveFrame(QWidget *parent = 0);
    ~LiveFrame();

protected:
    //override painting event to draw on the window
    void paintEvent(QPaintEvent *event);

private:
    Ui::LiveFrame *ui;

    int m_screenset;

    QPainter    *m_painter;
    QPen        *m_pen;
    QBrush      *m_brush;
};

#endif // LIVEFRAME_HPP
