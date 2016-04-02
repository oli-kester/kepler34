#include "EditKeys.hpp"

EditKeys::EditKeys(MidiSequence *a_seq, QWidget *parent)
    : m_seq(a_seq),
      QWidget(parent)
{

}

void EditKeys::paintEvent(QPaintEvent *)
{
    m_gc->set_foreground(m_black);
    m_pixmap->draw_rectangle(m_gc,true,
                             0,
                             0,
                             c_keyarea_x,
                             c_keyarea_y  );

    m_gc->set_foreground(m_white);
    m_pixmap->draw_rectangle(m_gc,true,
                             1,
                             1,
                             c_keyoffset_x - 1,
                             c_keyarea_y - 2  );


    for ( int i=0; i<c_num_keys; i++ )
    {
        m_gc->set_foreground(m_white);
        m_pixmap->draw_rectangle(m_gc,true,
                                 c_keyoffset_x + 1,
                                 (c_key_y * i) + 1,
                                 c_key_x - 2,
                                 c_key_y - 1 );

        /* the the key in the octave */
        int key = (c_num_keys - i - 1) % 12;

        if ( key == 1 ||
             key == 3 ||
             key == 6 ||
             key == 8 ||
             key == 10 ){

            m_gc->set_foreground(m_black);
            m_pixmap->draw_rectangle(m_gc,true,
                                     c_keyoffset_x + 1,
                                     (c_key_y * i) + 2,
                                     c_key_x - 3,
                                     c_key_y - 3 );
        }

        char notes[20];

        if ( key == m_key  ){



            /* notes */
            int octave = ((c_num_keys - i - 1) / 12) - 1;
            if ( octave < 0 )
                octave *= -1;

            snprintf(notes, sizeof(notes), "%2s%1d", c_key_text[key], octave);

            p_font_renderer->render_string_on_drawable(m_gc,
                                                       2,
                                                       c_key_y * i - 1,
                                                       m_pixmap, notes, font::BLACK );
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
