#ifndef EDITFRAME_HPP
#define EDITFRAME_HPP

#include <QFrame>

namespace Ui {
class EditFrame;
}

class EditFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EditFrame(QWidget *parent = 0);
    ~EditFrame();

private:
    Ui::EditFrame *ui;
};

#endif // EDITFRAME_HPP
