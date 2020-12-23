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

private:
    Ui::ueditrequest *ui;
    QSqlQueryModel *m_SystemType;
    QSortFilterProxyModel *m_SystemProxy;
    QSqlQueryModel *m_StatusModel;
    QSortFilterProxyModel *m_StatusProxy;

    int userID;
    int requestID;
    int objectID;

    struct requestData{
        int typeID;
        QString txtContext;
        bool isChanged {false};
    };

    struct objectData{
        QString txtName;
        QString txtAddress;
        QString txtPerson;
        QString txtTelephone;
        QString txtEmail;
        bool isChanged {false};
    };

    struct statusData{
        int typeID;
        QString txtComment;
        bool isChanged {false};
    };

    requestData chgRequestData;
    objectData chgObjectData;
    statusData chgStatusData;

    void SaveDialogSettings();
    void LoadDialogSettings();
    void StartInit();

    bool CheckRequestChange();
    bool CheckObjectChange();
    bool CheckStatusChange();

signals:
    void db_RequestUpdate();

};

#endif // UEDITREQUEST_H
