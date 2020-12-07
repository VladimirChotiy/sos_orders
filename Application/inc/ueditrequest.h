#ifndef UEDITREQUEST_H
#define UEDITREQUEST_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDataWidgetMapper>
#include <QSortFilterProxyModel>
#include "clDBMainQueryModel.h"

namespace Ui {
class ueditrequest;
}

class UEditRequest : public QDialog
{
    Q_OBJECT

public:
    explicit UEditRequest(int curUser, int curReq, QWidget *parent = nullptr);
    ~UEditRequest();
    void UpdateRequest(int userIndex);

private slots:
    void on_ueditrequest_accepted();
    void on_ueditrequest_rejected();
    void upd_RequestModelIndex_changed(int index);

private:
    Ui::ueditrequest *ui;
    QSqlQueryModel *m_RequestModel;
    QDataWidgetMapper *m_RequestMapper;
    QSqlQueryModel *m_ObjectModel;
    QSortFilterProxyModel *m_ObjectProxy;
    QSqlQueryModel *m_SystemType;
    QSortFilterProxyModel *m_SystemProxy;

    int userID;
    int requestID;

    void SaveDialogSettings();
    void LoadDialogSettings();
    void StartInit();

signals:
    void db_RequestUpdate();

};

#endif // UEDITREQUEST_H
