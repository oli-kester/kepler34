#ifndef SONGTIMEBAR_HPP
#define SONGTIMEBAR_HPP

#include <QWidget>

///
/// \brief The SongTimeBar class
///
/// The time bar for the song editor

class SongTimeBar : public QWidget
{
    Q_OBJECT
public:
    explicit SongTimeBar(QWidget *parent = 0);

protected:
    //override painting event to draw on the frame
    void paintEvent         (QPaintEvent *);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

    //override the sizehint to set our own defaults
    QSize sizeHint() const;

private:

    QTimer      *m_timer;
    QPen        *m_pen;
    QBrush      *m_brush;
    QPainter    *m_painter;
    QFont        m_font;


signals:

public slots:
};

#endif // SONGTIMEBAR_HPP
