#include "waitingdlg.h"
#include "ui_waitingdlg.h"

WaitingDlg::WaitingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDlg)
{
    ui->setupUi(this);
}

WaitingDlg::~WaitingDlg()
{
    delete ui;
}
