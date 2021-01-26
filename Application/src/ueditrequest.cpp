#include "ueditrequest.h"
#include "ui_ueditrequest.h"
#include "DBProcessor.h"
#include "settings.h"
#include "clDBReqInserter.h"
#include <QDebug>

UEditRequest::UEditRequest(int curUser, int curReq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ueditrequest),
    userID(curUser),
    requestID(curReq)
{
    StartInit();
}

UEditRequest::~UEditRequest()
{
    delete ui;
}

void UEditRequest::UpdateRequest(int userIndex)
{
    bool dataUpdate {false};
    db::clDBReqInserter *m_DBInserter {new db::clDBReqInserter(this)};

    QVariantList sendArgs {};
    if (CheckRequestChange()){
        sendArgs.clear();
        sendArgs << m_SystemType->data(m_SystemType->index(ui->cb_Type->currentIndex(), 0), Qt::DisplayRole).toInt();
        sendArgs << ui->txt_Context->toPlainText();
        m_DBInserter->UpdateData(requestID, sendArgs, DBTypes::DBUpdateType::Request);
        dataUpdate = true;
    }

    if (CheckObjectChange()) {
        sendArgs.clear();
        sendArgs << ui->ed_Object->text();
        sendArgs << ui->txt_Address->toPlainText();
        sendArgs << ui->ed_Person->text();
        sendArgs << ui->ed_Telephone->text();
        sendArgs << ui->ed_Email->text();
        m_DBInserter->UpdateData(objectID, sendArgs, DBTypes::DBUpdateType::Object);
        dataUpdate = true;
    }

    if (dataUpdate){
        sendArgs.clear();
        sendArgs << 11;
        sendArgs << requestID;
        sendArgs << "Авто: Данные в заявке были изменены";
        sendArgs << userIndex;
        m_DBInserter->AddData(sendArgs, DBTypes::DBInsertType::Status);
    }

    delete m_DBInserter;
}

void UEditRequest::on_ueditrequest_accepted()
{
    SaveDialogSettings();
    UpdateRequest(userID);
    emit db_RequestUpdate();
    close();
}

void UEditRequest::on_ueditrequest_rejected()
{
    close();
}

void UEditRequest::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("EditRequestDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void UEditRequest::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("EditRequestDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void UEditRequest::StartInit()
{
     ui->setupUi(this);
     LoadDialogSettings();

    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};

    m_SystemType = new QSqlQueryModel(this);
    m_SystemType->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::SystemType));
    ui->cb_Type->setModel(m_SystemType);
    ui->cb_Type->setModelColumn(1);

    m_StatusModel = new QSqlQueryModel(this);
    m_StatusModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Status, 1));
    ui->cb_Status->setModel(m_StatusModel);
    ui->cb_Status->setModelColumn(1);

    QSqlQuery RequestQuery {m_DBProcessor->prepareQuery(DBTypes::QueryType::RequestEdit, requestID)};
    delete m_DBProcessor;
    RequestQuery.next();
    ui->ed_ID->setText(RequestQuery.value(0).toString());
    ui->txt_Context->setText(RequestQuery.value(1).toString());
    ui->txt_Comment->setText(RequestQuery.value(2).toString());
    ui->ed_Object->setText(RequestQuery.value(4).toString());
    ui->txt_Address->setText(RequestQuery.value(5).toString());
    ui->ed_Person->setText(RequestQuery.value(6).toString());
    ui->ed_Telephone->setText(RequestQuery.value(7).toString());
    ui->ed_Email->setText(RequestQuery.value(8).toString());

    m_SystemProxy = new QSortFilterProxyModel(this);
    m_SystemProxy->setSourceModel(m_SystemType);
    m_SystemProxy->setFilterKeyColumn(0);
    m_SystemProxy->setFilterRegExp(QString::number(RequestQuery.value(3).toInt()));
    int cbTypeRow {m_SystemProxy->mapToSource(m_SystemProxy->index(0, 0)).row()};
    ui->cb_Type->setCurrentIndex(cbTypeRow);
    delete m_SystemProxy;

    m_StatusProxy = new QSortFilterProxyModel(this);
    m_StatusProxy->setSourceModel(m_StatusModel);
    m_StatusProxy->setFilterKeyColumn(0);
    m_StatusProxy->setFilterRegExp(QString::number(RequestQuery.value(9).toInt()));
    int cbStatusRow {m_StatusProxy->mapToSource(m_StatusProxy->index(0, 0)).row()};
    ui->cb_Status->setCurrentIndex(cbStatusRow);

    objectID = RequestQuery.value(10).toInt();

    chgRequestData.txtContext = ui->txt_Context->toPlainText();
    chgRequestData.typeID = m_SystemType->data(m_SystemType->index(ui->cb_Type->currentIndex(), 0), Qt::DisplayRole).toInt();

    chgObjectData.txtName = ui->ed_Object->text();
    chgObjectData.txtPerson = ui->ed_Person->text();
    chgObjectData.txtAddress = ui->txt_Address->toPlainText();
    chgObjectData.txtTelephone = ui->ed_Telephone->text();
    chgObjectData.txtEmail = ui->ed_Email->text();

    chgStatusData.txtComment = ui->txt_Comment->toPlainText();
    chgStatusData.typeID = 0;
}

bool UEditRequest::CheckRequestChange()
{
    int typeID {m_SystemType->data(m_SystemType->index(ui->cb_Type->currentIndex(), 0), Qt::DisplayRole).toInt()};
    if ((ui->txt_Context->toPlainText() != chgRequestData.txtContext) || (typeID != chgRequestData.typeID)){
        chgRequestData.isChanged = true;
    }else {
        chgRequestData.isChanged = false;
    }
    return chgRequestData.isChanged;
}

bool UEditRequest::CheckObjectChange()
{
    if ((ui->ed_Object->text() != chgObjectData.txtName) || (ui->ed_Person->text() != chgObjectData.txtPerson) || (ui->txt_Address->toPlainText() != chgObjectData.txtAddress) || (ui->ed_Telephone->text() != chgObjectData.txtTelephone) || (ui->ed_Email->text() != chgObjectData.txtEmail)){
        chgObjectData.isChanged = true;
    }else {
        chgObjectData.isChanged = false;
    }
    return chgObjectData.isChanged;
}

bool UEditRequest::CheckStatusChange()
{
    return chgStatusData.isChanged;
}
