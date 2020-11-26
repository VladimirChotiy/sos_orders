#ifndef UICHANGESTATUS_H
#define UICHANGESTATUS_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class uiChangeStatus;
}

class uiChangeStatus : public QDialog
{
    Q_OBJECT

public:
    explicit uiChangeStatus(std::pair<int, int> indexes, int state, QWidget *parent = nullptr);
    ~uiChangeStatus();

private:
    Ui::uiChangeStatus *ui;
    QSqlQueryModel *m_StatusModel;

    int reqID {-1};
    int userID {-1};
    int curState {2};
    void StartInit();
    void SaveDialogSettings();
    void LoadDialogSettings();
    void ChangeStatus();

signals:
    void usr_ReqStatus_update();

private slots:
    void on_uiChangeStatus_accepted();
    void on_uiChangeStatus_rejected();
};

#endif // UICHANGESTATUS_H
