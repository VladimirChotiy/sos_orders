#ifndef UREGISTERNEWREQUEST_H
#define UREGISTERNEWREQUEST_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>

namespace Ui {
class URegisterNewRequest;
}

class URegisterNewRequest : public QDialog
{
    Q_OBJECT

public:
    explicit URegisterNewRequest(QWidget *parent = nullptr);
    ~URegisterNewRequest();

private slots:
    void on_URegisterNewRequest_rejected();
    void on_URegisterNewRequest_accepted();
    void usr_PersonData_changed(int index);

    void on_btn_AddPerson_clicked();

    void on_btn_AddObject_clicked();

private:
    Ui::URegisterNewRequest *ui;
    QSqlTableModel *m_personListModel;
    QSqlQueryModel *m_ObjectModel;
    QDataWidgetMapper *m_personMapper;
    QDataWidgetMapper *m_ObjectMapper;
    QSqlTableModel *m_systemType;
    void SaveDialogSettings();
    void LoadDialogSettings();
    void StartInit();
};

#endif // UREGISTERNEWREQUEST_H
