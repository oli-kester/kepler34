#ifndef SONGSEQUENCENAMES_HPP
#define SONGSEQUENCENAMES_HPP

#include <QWidget>

class SongSequenceNames : public QWidget
{
    Q_OBJECT
public:
    explicit SongSequenceNames(QWidget *parent = 0);

protected:
    //override painting event to draw on the frame
    void paintEvent         (QPaintEvent *);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

    //override the sizehint to set our own defaults
    QSize sizeHint() const;

signals:

public slots:

private:

    QTimer      *m_timer;
    QPen        *m_pen;
    QBrush      *m_brush;
    QPainter    *m_painter;
    QFont        m_font;
};

#endif // SONGSEQUENCENAMES_HPP
