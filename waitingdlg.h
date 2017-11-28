#ifndef WAITINGDLG_H
#define WAITINGDLG_H

#include <QDialog>

namespace Ui {
class WaitingDlg;
}

class WaitingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDlg(QWidget *parent = 0);
    ~WaitingDlg();

private:
    Ui::WaitingDlg *ui;
};

#endif // WAITINGDLG_H
