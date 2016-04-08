#include "SongSequenceNames.hpp"

SongSequenceNames::SongSequenceNames(MidiPerformance *a_perf,
                                     QWidget *parent) :
    QWidget(parent),
    m_mainperf(a_perf)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::MinimumExpanding);

    for( int i=0; i<c_total_seqs; ++i )
        m_sequence_active[i]=false;
}

void SongSequenceNames::paintEvent(QPaintEvent *)
{
    mPainter = new QPainter(this);
    mPen = new QPen(Qt::black);
    mBrush = new QBrush(Qt::SolidPattern);
    mBrush->setColor(Qt::black);
    mFont.setPointSize(6);
    mPainter->setPen(*mPen);
    mPainter->setBrush(*mBrush);
    mPainter->setFont(mFont);

    int y_s = 0;
    int y_f = height() / c_names_y;

    for ( int y = y_s; y <= y_f; y++ )
    {
        int sequence = y;

        if (sequence < c_total_seqs)
        {
            int i = sequence;

            //black boxes to mark each bank
            mPen->setColor(Qt::black);
            mPainter->setPen(*mPen);
            mPainter->drawRect(0,
                               (c_names_y * i),
                               c_names_x,
                               c_names_y + 1);

            //if first seq bank
            if ( sequence % c_seqs_in_set == 0 )
            {
                char ss[3];
                snprintf(ss, sizeof(ss), "%2d", sequence / c_seqs_in_set );
                //draw bank number here
                mPen->setColor(Qt::white);
                mPainter->setPen(*mPen);
                mPainter->drawText(4,
                                   c_names_y * i + 15,
                                   ss);
            }
            else
            {
                //no number here, just plain sidebar
                mPen->setStyle(Qt::NoPen);
                mBrush->setColor(Qt::lightGray);
                mPainter->setPen(*mPen);
                mPainter->setBrush(*mBrush);
                mPainter->drawRect(1,
                                   (c_names_y * (i)),
                                   (6*2) + 1,
                                   c_names_y);
            }

            mPen->setStyle(Qt::SolidLine);
            if ( m_mainperf->is_active( sequence ))
                mPen->setColor(Qt::white);
            else
                mPen->setColor(Qt::gray);

            // fill seq label background
            mPainter->setPen(*mPen);
            mPainter->drawRect(6 * 2 + 4,
                               (c_names_y * i) + 1,
                               c_names_x - 3 - (6*2),
                               c_names_y - 1);

            if ( m_mainperf->is_active( sequence )){

                m_sequence_active[sequence]=true;

                //draw seq info on label
                char name[50];
                snprintf(name, sizeof(name), "%-14.14s                                %2d",
                         m_mainperf->get_sequence(sequence)->get_name(),
                         m_mainperf->get_sequence(sequence)->get_midi_channel() + 1);

                //seq name
                mPen->setColor(Qt::black);
                mPainter->setPen(*mPen);
                mPainter->drawText(5 + 6*2,
                                   c_names_y * i + 10,
                                   name);

                char str[20];
                snprintf(str, sizeof(str),
                         "%d-%d %ld/%ld",
                         m_mainperf->get_sequence(sequence)->get_midi_bus(),
                         m_mainperf->get_sequence(sequence)->get_midi_channel()+1,
                         m_mainperf->get_sequence(sequence)->getBeatsPerMeasure(),
                         m_mainperf->get_sequence(sequence)->getBeatWidth());

                //seq info
                mPainter->drawText(5 + 6*2,
                                   c_names_y * i + 20,
                                   str);

                bool muted = m_mainperf->get_sequence(sequence)->get_song_mute();

                mPen->setColor(Qt::black);
                mPainter->setPen(*mPen);
                mPainter->drawRect(6*2 + 6 * 20 + 2,
                                   (c_names_y * i),
                                   10,
                                   c_names_y  );

                //seq mute state
                if ( muted )
                {
                    mPainter->drawText(5 + 6*2 + 6 * 20,
                                       c_names_y * i + 14,
                                       "M");
                }
                else
                {
                    mPainter->drawText(5 + 6*2 + 6 * 20,
                                       c_names_y * i + 14,
                                       "M");
                }
            }
        }
    }
}


QSize SongSequenceNames::sizeHint() const
{
    return QSize(c_names_x, 30000);
}

void SongSequenceNames::mousePressEvent(QMouseEvent *event)
{

}

void SongSequenceNames::mouseReleaseEvent(QMouseEvent *event)
{

}

void SongSequenceNames::mouseMoveEvent(QMouseEvent *event)
{

}
