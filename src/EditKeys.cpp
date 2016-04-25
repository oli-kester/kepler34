#include "EditKeys.hpp"

EditKeys::EditKeys(MidiSequence *a_seq, QWidget *parent, int keyHeight, int keyAreaHeight):
    m_seq(a_seq),
    QWidget(parent),
    m_key(0),
    keyY(keyHeight),
    keyAreaY(keyAreaHeight)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::MinimumExpanding);
}

void EditKeys::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::black);
    m_brush = new QBrush(Qt::SolidPattern);
    m_brush->setColor(Qt::lightGray);
    m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setBrush(*m_brush);
    m_painter->setFont(m_font);

    //draw keyboard border
    m_painter->drawRect(0,
                        0,
                        c_keyarea_x,
                        keyAreaY);

    for ( int i = 0; i < c_num_keys; i++ )
    {
        //draw white keys
        m_pen->setColor(Qt::black);
        m_painter->setPen(*m_pen);
        m_brush->setColor(Qt::white);
        m_brush->setStyle(Qt::SolidPattern);
        m_painter->setBrush(*m_brush);
        m_painter->drawRect(c_keyoffset_x + 1,
                            keyY * i + 1,
                            c_key_x - 2,
                            keyY - 1);

        /* the the key in the octave */
        int key = (c_num_keys - i - 1) % 12;

        if ( key == 1 ||
             key == 3 ||
             key == 6 ||
             key == 8 ||
             key == 10 )
        {
            //draw black keys
            m_pen->setColor(Qt::black);
            m_brush->setColor(Qt::black);
            m_painter->setPen(*m_pen);
            m_painter->setBrush(*m_brush);
            m_painter->drawRect(c_keyoffset_x + 1,
                                keyY * i + 3,
                                c_key_x - 4,
                                keyY - 5);
        }

        char notes[20];

        if ( key == m_key  )
        {
            /* notes */
            int octave = ((c_num_keys - i - 1) / 12) - 1;
            if ( octave < 0 )
                octave *= -1;

            snprintf(notes, sizeof(notes), "%2s%1d", c_key_text[key], octave);

            //draw "Cx" octave labels
            m_pen->setColor(Qt::black);
            m_painter->setPen(*m_pen);
            m_painter->drawText(2, keyY * i + 11, notes);
        }

    }
    delete m_painter;
    delete m_brush;
    delete m_pen;
}

void EditKeys::mousePressEvent(QMouseEvent *event)
{

}

void EditKeys::mouseReleaseEvent(QMouseEvent *event)
{

}

void EditKeys::mouseMoveEvent(QMouseEvent *event)
{

}

QSize EditKeys::sizeHint() const
{
    return QSize(c_keyarea_x, keyAreaY);
}
