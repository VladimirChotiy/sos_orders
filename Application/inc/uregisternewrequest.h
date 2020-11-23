#ifndef UREGISTERNEWREQUEST_H
#define UREGISTERNEWREQUEST_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

namespace Ui {
class URegisterNewRequest;
}

class URegisterNewRequest : public QDialog
{
    Q_OBJECT

public:
    explicit URegisterNewRequest(int userID, QWidget *parent = nullptr);
    ~URegisterNewRequest();

private slots:
    void on_URegisterNewRequest_rejected();
    void on_URegisterNewRequest_accepted();
    void usr_PersonData_changed(int index);
    void usr_ObjectData_changed(int index);
    void usr_SystemData_changed(int index);
    void on_btn_ClearPerson_clicked();
    void on_btn_ClearObj_clicked();

private:
    Ui::URegisterNewRequest *ui;
    QSqlQueryModel *m_personListModel;
    QDataWidgetMapper *m_personMapper;
    QSqlQueryModel *m_ObjectModel;
    QDataWidgetMapper *m_ObjectMapper;
    QSqlQueryModel *m_systemType;
    QDataWidgetMapper *m_SystemMapper;
    void SaveDialogSettings();
    void LoadDialogSettings();
    void StartInit();
    int dbUserID;

signals:
    void db_NewRequest_Add(void);
};

#endif // UREGISTERNEWREQUEST_H
