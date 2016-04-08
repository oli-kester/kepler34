#ifndef SONGSEQUENCEGRID_HPP
#define SONGSEQUENCEGRID_HPP

#include <QWidget>
#include <QTimer>
#include <QObject>
#include <QPainter>
#include <QPen>

class SongSequenceGrid : public QWidget
{
    Q_OBJECT
public:
    explicit SongSequenceGrid(QWidget *parent = 0);

    int snap() const;
    void setSnap(int snap);

protected:
    //override painting event to draw on the frame
    void paintEvent         (QPaintEvent *);

    //override mouse events for interaction
    void mousePressEvent    (QMouseEvent * event);
    void mouseReleaseEvent  (QMouseEvent * event);
    void mouseMoveEvent     (QMouseEvent * event);

    //override keyboard events for interaction
    void keyPressEvent      (QKeyEvent * event);
    void keyReleaseEvent    (QKeyEvent * event);

    //override the sizehint to set our own defaults
    QSize sizeHint() const;

signals:

public slots:

private:

    QPen        *mPen;
    QBrush      *mBrush;
    QPainter    *mPainter;
    QFont        mFont;
    QTimer      *mTimer;

    int m_snap;

};

#endif // SONGSEQUENCEGRID_HPP
