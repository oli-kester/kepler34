#ifndef SONGFRAME_HPP
#define SONGFRAME_HPP

#include <QFrame>
#include <qmath.h>

namespace Ui {
class SongFrame;
}

class SongFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SongFrame(QWidget *parent = 0);
    ~SongFrame();

private:
    Ui::SongFrame *ui;
};

#endif // SONGFRAME_HPP
