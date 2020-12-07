#ifndef UICHOOSEENGINEER_H
#define UICHOOSEENGINEER_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class uiChooseEngineer;
}

class uiChooseEngineer : public QDialog
{
    Q_OBJECT

public:
    explicit uiChooseEngineer(std::pair<int, int> indexes, QWidget *parent = nullptr);
    ~uiChooseEngineer();

private slots:
    void on_uiChooseEngineer_accepted();
    void on_uiChooseEngineer_rejected();

private:
    Ui::uiChooseEngineer *ui;
    QSqlQueryModel *m_EngineerModel;

    int reqID {-1};
    int userID {-1};
    void StartInit();
    void SaveDialogSettings();
    void LoadDialogSettings();
    void ChangeEng();

signals:
    void usr_Engineer_update();
};

#endif // UICHOOSEENGINEER_H
