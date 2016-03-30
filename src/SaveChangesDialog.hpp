#ifndef SAVECHANGESDIALOG_HPP
#define SAVECHANGESDIALOG_HPP

#include <QDialog>

namespace Ui {
class SaveChangesDialog;
}

class SaveChangesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveChangesDialog(QWidget *parent = 0);
    ~SaveChangesDialog();

private:
    Ui::SaveChangesDialog *ui;
};

#endif // SAVECHANGESDIALOG_HPP
