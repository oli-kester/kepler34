#include "EditTimeBar.hpp"

EditTimeBar::EditTimeBar(MidiSequence *a_seq, QWidget *parent):
    QWidget(parent),

    m_seq(a_seq)
{
    //start refresh timer to queue regular redraws
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    m_timer->start();

    m_zoom = 1;

    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);
}

void EditTimeBar::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::black);
    m_brush = new QBrush(Qt::lightGray, Qt::SolidPattern),
            m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setBrush(*m_brush);
    m_painter->setFont(m_font);

    //draw time bar border
    m_painter->drawRect(0,
                        0,
                        size().width(),
                        size().height());

    int measure_length_32nds =  m_seq->getBeatsPerMeasure() * 32 /
            m_seq->getBeatWidth();

    int measures_per_line = (128 / measure_length_32nds) / (32 / m_zoom);
    if ( measures_per_line <= 0 )
        measures_per_line = 1;

    int ticks_per_measure =  m_seq->getBeatsPerMeasure() * (4 * c_ppqn) / m_seq->getBeatWidth();
    int ticks_per_beat =  ticks_per_measure * measures_per_line;
    int start_tick = 0;
    int end_tick = (m_seq->getLength());

    m_pen->setColor(Qt::black);
    m_painter->setPen(*m_pen);
    for (int i = start_tick; i <= end_tick; i += ticks_per_beat)
    {
        int zoomedX = i / m_zoom;

        //vertical line at each beat
        m_painter->drawLine(zoomedX,
                            0,
                            zoomedX,
                            size().height());

        char bar[5];
        snprintf(bar, sizeof(bar), "%d", (i/ ticks_per_measure ) + 1);

        //number each beat
        m_pen->setColor(Qt::black);
        m_painter->setPen(*m_pen);
        m_painter->drawText(zoomedX + 3,
                            10,
                            bar);

    }

    long end_x = m_seq->getLength() / m_zoom;

    //draw end of seq label
    //label background
    m_pen->setColor(Qt::white);
    m_brush->setColor(Qt::white);
    m_brush->setStyle(Qt::SolidPattern);
    m_painter->setBrush(*m_brush);
    m_painter->setPen(*m_pen);
    m_painter->drawRect(end_x + 1,
                        13,
                        15,
                        8);
    //label text
    m_pen->setColor(Qt::black);
    m_painter->setPen(*m_pen);
    m_painter->drawText(end_x + 1, 21,
                        tr("END"));

    delete m_painter;
    delete m_brush;
    delete m_pen;
}

void EditTimeBar::mousePressEvent(QMouseEvent *event)
{

}

void EditTimeBar::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditTimeBar::mouseMoveEvent(QMouseEvent *event)
{

}

QSize EditTimeBar::sizeHint() const
{
    return QSize(m_seq->getLength() / m_zoom + 100, 22);
}

void EditTimeBar::zoomIn()
{
    if (m_zoom > 1)
        m_zoom *= 0.5;
}

void EditTimeBar::zoomOut()
{
    if (m_zoom < 32)
        m_zoom *= 2;
}
