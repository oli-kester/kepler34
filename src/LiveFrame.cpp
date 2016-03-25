#include "LiveFrame.hpp"
#include "ui_LiveFrame.h"

LiveFrame::LiveFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LiveFrame)
{
    ui->setupUi(this);

    m_screenset = 0;
}

void LiveFrame::paintEvent(QPaintEvent *event)
{
    static const QPoint points[4] = {
        QPoint(10, 80),
        QPoint(20, 10),
        QPoint(80, 30),
        QPoint(90, 70)
    };

    QRect rect(10, 20, 80, 60);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;

    QPainter painter(this);
//    painter.setPen(m_pen);
//    painter.setBrush(m_brush);

    for (int x = 0; x < width(); x += 100) {
        for (int y = 0; y < height(); y += 100) {
            painter.save();
            painter.translate(x, y);
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);

                painter.drawRect(rect);
            painter.restore();
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

LiveFrame::~LiveFrame()
{
    delete ui;
}
