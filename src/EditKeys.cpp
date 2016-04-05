#include "EditKeys.hpp"

EditKeys::EditKeys(MidiSequence *a_seq, QWidget *parent):
    m_seq(a_seq),
    QWidget(parent),
    m_key(0)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::MinimumExpanding);
}

void EditKeys::paintEvent(QPaintEvent *)
{
    m_painter = new QPainter(this);
    m_pen = new QPen(Qt::black);
    m_font.setPointSize(6);
    m_painter->setPen(*m_pen);
    m_painter->setFont(m_font);

    m_rect = QRect(0,
                   0,
                   c_keyarea_x,
                   c_keyarea_y);
    m_painter->drawRect(m_rect);

    m_pen = new QPen(Qt::white);
    m_painter->setPen(*m_pen);
    m_rect = QRect(1,
                   1,
                   c_keyoffset_x - 1,
                   c_keyarea_y - 2  );
    m_painter->drawRect(m_rect);

    for ( int i = 0; i < c_num_keys; i++ )
    {
        m_pen = new QPen(Qt::white);
        m_painter->setPen(*m_pen);
        m_rect = QRect(c_keyoffset_x + 1,
                       (c_key_y * i) + 1,
                       c_key_x - 2,
                       c_key_y - 1 );
        m_painter->drawRect(m_rect);

        /* the the key in the octave */
        int key = (c_num_keys - i - 1) % 12;

        if ( key == 1 ||
             key == 3 ||
             key == 6 ||
             key == 8 ||
             key == 10 )
        {

            m_pen = new QPen(Qt::black);
            m_painter->setPen(*m_pen);
            m_rect = QRect(c_keyoffset_x + 1,
                           (c_key_y * i) + 2,
                           c_key_x - 3,
                           c_key_y - 3 );
            m_painter->drawRect(m_rect);
        }

        char notes[20];

        if ( key == m_key  )
        {
            /* notes */
            int octave = ((c_num_keys - i - 1) / 12) - 1;
            if ( octave < 0 )
                octave *= -1;

            snprintf(notes, sizeof(notes), "%2s%1d", c_key_text[key], octave);

            m_pen = new QPen(Qt::black);
            m_painter->setPen(*m_pen);
            m_point = QPoint(2, c_key_y * i + 9);
            m_painter->drawText(m_point, notes);
        }

    }
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
    return QSize(40,5000);
}
