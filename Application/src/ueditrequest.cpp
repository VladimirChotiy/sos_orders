#include "ueditrequest.h"
#include "ui_ueditrequest.h"
#include "DBProcessor.h"
#include "settings.h"
#include "clDBReqInserter.h"

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
    Q_UNUSED(userIndex)

    QVariantList sendArgs {};
    sendArgs << m_ObjectModel->query().value(0).toInt();
    sendArgs << m_SystemType->query().value(0).toInt();
    sendArgs << ui->txt_Context->toPlainText();
    sendArgs << ui->txt_Comment->toPlainText();

    db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
    m_ReqInserter->UpdateData(requestID, sendArgs, DBTypes::DBUpdateType::Request);
    delete m_ReqInserter;
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

void UEditRequest::upd_RequestModelIndex_changed(int index)
{
    //Q_UNUSED(index)

    int typeID {m_RequestModel->data(m_RequestModel->index(index,3)).toInt()};
    m_SystemProxy->setFilterRegExp(QString::number(typeID));
    int cbRow {m_SystemProxy->mapToSource(m_SystemProxy->index(0,0)).row()};
    ui->cb_Type->setCurrentIndex(cbRow);

    int objID {m_RequestModel->data(m_RequestModel->index(index,4)).toInt()};
    m_ObjectProxy->setFilterRegExp(QString::number(objID));
    cbRow = m_ObjectProxy->mapToSource(m_ObjectProxy->index(0,0)).row();
    ui->cb_Object->setCurrentIndex(cbRow);
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

     m_RequestModel = new QSqlQueryModel(this);
     m_RequestModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::RequestEdit, requestID));
     m_RequestMapper = new QDataWidgetMapper(this);
     m_RequestMapper->setModel(m_RequestModel);
     m_RequestMapper->addMapping(ui->ed_ID, 0);
     m_RequestMapper->addMapping(ui->txt_Context, 1);
     m_RequestMapper->addMapping(ui->txt_Comment, 2);
     m_RequestMapper->addMapping(ui->ed_Status, 6);
     m_RequestMapper->addMapping(ui->ed_Date, 7);
     QObject::connect(m_RequestMapper, SIGNAL(currentIndexChanged(int)), this, SLOT(upd_RequestModelIndex_changed(int)));


     m_SystemType = new QSqlQueryModel(this);
     m_SystemType->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::SystemType));
     ui->cb_Type->setModel(m_SystemType);
     ui->cb_Type->setModelColumn(1);
     m_SystemProxy = new QSortFilterProxyModel(this);
     m_SystemProxy->setSourceModel(m_SystemType);
     m_SystemProxy->setFilterKeyColumn(0);

     m_ObjectModel = new QSqlQueryModel(this);
     m_ObjectModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Object));
     ui->cb_Object->setModel(m_ObjectModel);
     ui->cb_Object->setModelColumn(1);
     m_ObjectProxy = new QSortFilterProxyModel(this);
     m_ObjectProxy->setSourceModel(m_ObjectModel);
     m_SystemProxy->setFilterKeyColumn(0);

     m_RequestMapper->toFirst();

     delete m_DBProcessor;
}
