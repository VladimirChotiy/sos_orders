#ifndef UISETCOST_H
#define UISETCOST_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAbstractButton;
QT_END_NAMESPACE

namespace Ui {
class uiSetCost;
}

class uiSetCost : public QDialog
{
    Q_OBJECT

public:
    explicit uiSetCost(std::pair<int, int> indexes, QWidget *parent = nullptr);
    ~uiSetCost();

private slots:
    void on_uiSetCost_accepted();
    void on_uiSetCost_rejected();
    void on_bb_Dialog_clicked(QAbstractButton *button);

private:
    Ui::uiSetCost *ui;

    int reqID {-1};
    int userID {-1};
    void StartInit();
    void SaveDialogSettings();
    void LoadDialogSettings();
    void ChangeCost();

signals:
    void usr_ReqCost_update();
};

#endif // UISETCOST_H
