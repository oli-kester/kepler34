#include "SongSequenceGrid.hpp"

SongSequenceGrid::SongSequenceGrid(MidiPerformance *a_perf,
                                   QWidget *parent):
    QWidget(parent),
    m_mainperf(a_perf),
    m_roll_length_ticks(0),
    m_drop_sequence(0),
    m_moving(false),
    m_growing(false)
{
    setSizePolicy(QSizePolicy::Fixed,
                  QSizePolicy::Fixed);

    for( int i=0; i<c_total_seqs; ++i )
        m_sequence_active[i]=false;

    m_roll_length_ticks = m_mainperf->get_max_trigger();
    m_roll_length_ticks = m_roll_length_ticks -
            ( m_roll_length_ticks % ( c_ppqn * 16 ));
    m_roll_length_ticks +=  c_ppqn * 4096;

    //start refresh timer to queue regular redraws
    mTimer = new QTimer(this);
    mTimer->setInterval(1);
    QObject::connect(mTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(update()));
    mTimer->start();
}

void SongSequenceGrid::paintEvent(QPaintEvent *)
{
    mPainter = new QPainter(this);
    mBrush = new QBrush(Qt::NoBrush);
    mPen = new QPen(Qt::black);
    mPen->setStyle(Qt::DashLine);
    mFont.setPointSize(6);
    mPainter->setPen(*mPen);
    mPainter->setBrush(*mBrush);
    mPainter->setFont(mFont);

    /* draw horz grey lines */

    mPainter->drawLine(0,
                       0,
                       c_perfroll_background_x,
                       0);

    int beats = m_measure_length / m_beat_length;

    /* draw vert lines */
    for ( int i=0; i< beats ; ){

        if ( i == 0 ){
            mPen->setStyle(Qt::SolidLine);
        }
        else
        {
            mPen->setStyle(Qt::DashLine);
        }

        /* solid line on every beat */
        mPainter->setPen(*mPen);
        mPainter->drawRect(i * m_beat_length / c_perf_scale_x,
                           0,
                           i * m_beat_length / c_perf_scale_x,
                           c_names_y);

        // jump 2 if 16th notes
        if ( m_beat_length < c_ppqn/2 )
        {
            i += (c_ppqn / m_beat_length);
        }
        else
        {
            ++i;
        }

    }

    //draw playhead
    long tick = m_mainperf->get_tick();

    int progress_x = tick / c_perf_scale_x ;

    mPen->setColor(Qt::black);
    mPainter->setPen(*mPen);
    mPainter->drawLine(progress_x, 0,
                       progress_x, height());

    //draw background

    long first_measure = 0;

    int y_s = 0;
    int y_f = height() / c_names_y;

    //draw sequence block
    long tick_on;
    long tick_off;
    long offset;
    bool selected;

    long tick_offset = c_ppqn * 16;
    long x_offset = tick_offset / c_perf_scale_x;

    for ( int y = y_s; y <= y_f; y++ )
    {
        int a_sequence = y;

        y = c_names_y * a_sequence;
        int h = c_names_y;

        mPen->setColor(Qt::white);
        mPainter->setPen(*mPen);
        mPainter->drawRect(0,
                           y,
                           width(),
                           h);

        mPen->setColor(Qt::black);
        mPainter->setPen(*mPen);
        for ( int i = first_measure;
              i < first_measure +
              (width() * c_perf_scale_x /
               (m_measure_length)) + 1;

              i++ )
        {
            int x_pos = (i * m_measure_length) / c_perf_scale_x;

            //            a_draw->draw_drawable(m_gc, m_background,
            //                                  0,
            //                                  0,
            //                                  x_pos,
            //                                  y,
            //                                  c_perfroll_background_x,
            //                                  c_names_y );

        }

        if ( a_sequence < c_total_seqs ){

            if ( m_mainperf->is_active( a_sequence )){

                m_sequence_active[a_sequence] = true;

                MidiSequence *seq =  m_mainperf->get_sequence( a_sequence );

                seq->reset_draw_trigger_marker();

                long seq_length = seq->getLength();
                int length_w = seq_length / c_perf_scale_x;

                while ( seq->get_next_trigger( &tick_on, &tick_off, &selected, &offset  )){

                    if ( tick_off > 0 ){

                        long x_on  = tick_on  / c_perf_scale_x;
                        long x_off = tick_off / c_perf_scale_x;
                        int  w     = x_off - x_on + 1;

                        int x = x_on;
                        int y = c_names_y * a_sequence + 1;  // + 2
                        int h = c_names_y - 2; // - 4

                        // adjust to screen corrids
                        x = x - x_offset;

                        if ( selected )
                            mPen->setColor(Qt::gray);
                        else
                            mPen->setColor(Qt::gray);

                        mPainter->setPen(*mPen);
                        mPainter->drawRect(x,
                                           y,
                                           w,
                                           h);

                        mPen->setColor(Qt::gray);
                        mPainter->setPen(*mPen);
                        mPainter->drawRect(x,
                                           y,
                                           w,
                                           h);

                        mPen->setColor(Qt::black);
                        mPainter->setPen(*mPen);
                        mPainter->drawRect(x,
                                           y,
                                           c_perfroll_size_box_w,
                                           c_perfroll_size_box_w);

                        mPainter->drawRect(x+w-c_perfroll_size_box_w,
                                           y+h-c_perfroll_size_box_w,
                                           c_perfroll_size_box_w,
                                           c_perfroll_size_box_w);

                        mPen->setColor(Qt::black);
                        mPainter->setPen(*mPen);

                        long length_marker_first_tick = ( tick_on - (tick_on % seq_length) + (offset % seq_length) - seq_length);


                        long tick_marker = length_marker_first_tick;

                        while ( tick_marker < tick_off ){

                            long tick_marker_x = (tick_marker / c_perf_scale_x) - x_offset;

                            if ( tick_marker > tick_on ){

                                mPen->setColor(Qt::lightGray);
                                mPainter->setPen(*mPen);
                                mPainter->drawRect(tick_marker_x,
                                                   y+4,
                                                   1,
                                                   h-8);
                            }

                            int lowest_note = seq->get_lowest_note_event( );
                            int highest_note = seq->get_highest_note_event( );

                            int height = highest_note - lowest_note;
                            height += 2;

                            int length = seq->getLength( );

                            long tick_s;
                            long tick_f;
                            int note;

                            bool selected;

                            int velocity;
                            draw_type dt;

                            seq->reset_draw_marker();

                            mPen->setColor(Qt::black);
                            mPainter->setPen(*mPen);

                            while ( (dt = seq->get_next_note_event( &tick_s, &tick_f, &note,
                                                                    &selected, &velocity )) != DRAW_FIN ){

                                int note_y = ((c_names_y-6) -
                                              ((c_names_y-6)  * (note - lowest_note)) / height) + 1;

                                int tick_s_x = ((tick_s * length_w)  / length) + tick_marker_x;
                                int tick_f_x = ((tick_f * length_w)  / length) + tick_marker_x;

                                if ( dt == DRAW_NOTE_ON || dt == DRAW_NOTE_OFF )
                                    tick_f_x = tick_s_x + 1;
                                if ( tick_f_x <= tick_s_x )
                                    tick_f_x = tick_s_x + 1;

                                if ( tick_s_x < x ){
                                    tick_s_x = x;
                                }

                                if ( tick_f_x > x + w ){
                                    tick_f_x = x + w;
                                }

                                if ( tick_f_x >= x && tick_s_x <= x+w )
                                    mPainter->drawLine(tick_s_x,
                                                       y + note_y,
                                                       tick_f_x,
                                                       y + note_y);
                            }

                            tick_marker += seq_length;
                        }
                    }
                }
            }
        }
    }
}

int SongSequenceGrid::getSnap() const
{
    return m_snap;
}

void SongSequenceGrid::setSnap(int snap)
{
    m_snap = snap;
}

QSize SongSequenceGrid::sizeHint() const
{
    return QSize(5000, 5000);
}

void SongSequenceGrid::mousePressEvent(QMouseEvent *event)
{

}

void SongSequenceGrid::mouseReleaseEvent(QMouseEvent *event)
{

}

void SongSequenceGrid::mouseMoveEvent(QMouseEvent *event)
{

}

void SongSequenceGrid::keyPressEvent(QKeyEvent *event)
{

}

void SongSequenceGrid::keyReleaseEvent(QKeyEvent *event)
{

}

/* performs a 'snap' on x */
void SongSequenceGrid::snap_x( int *a_x )
{
    // snap = number pulses to snap to
    // m_scale = number of pulses per pixel
    //	so snap / m_scale  = number pixels to snap to

    int mod = (m_snap / c_perf_scale_x );

    if ( mod <= 0 )
        mod = 1;

    *a_x = *a_x - (*a_x % mod );
}


void SongSequenceGrid::convert_x( int a_x, long *a_tick )
{

    long tick_offset = c_ppqn * 16;
    *a_tick = a_x * c_perf_scale_x;
    *a_tick += tick_offset;
}


void SongSequenceGrid::convert_xy( int a_x, int a_y, long *a_tick, int *a_seq)
{

    long tick_offset =  c_ppqn * 16;

    *a_tick = a_x * c_perf_scale_x;
    *a_seq = a_y / c_names_y;

    *a_tick += tick_offset;

    if ( *a_seq >= c_total_seqs )
        *a_seq = c_total_seqs - 1;

    if ( *a_seq < 0 )
        *a_seq = 0;
}

void SongSequenceGrid::half_split_trigger( int a_sequence, long a_tick )
{
    m_mainperf->push_trigger_undo();
    m_mainperf->get_sequence( a_sequence )->half_split_trigger( a_tick );
}

/* simply sets the snap member */
void SongSequenceGrid::set_guides( int a_snap, int a_measure, int a_beat )
{
    m_snap = a_snap;
    m_measure_length = a_measure;
    m_beat_length = a_beat;
}
