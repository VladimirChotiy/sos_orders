#include "uregisternewrequest.h"
#include "ui_uregisternewrequest.h"
#include "DBProcessor.h"
#include "dbtypes.h"
#include "settings.h"
#include "clDBReqInserter.h"

URegisterNewRequest::URegisterNewRequest(int userID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URegisterNewRequest),
    dbUserID(userID)
{
    StartInit();
}

URegisterNewRequest::~URegisterNewRequest()
{
    delete ui;
}

void URegisterNewRequest::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("AddNewRequestDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void URegisterNewRequest::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("AddNewRequestDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void URegisterNewRequest::StartInit()
{
    ui->setupUi(this);
    ui->ed_Date->setDate(QDate::currentDate());
    LoadDialogSettings();

    m_personListModel = new QSqlQueryModel(this);
    m_personMapper = new QDataWidgetMapper(this);
    m_personListModel->setQuery(prepareQuery(QueryType::Person));
    ui->cb_Person->setModel(m_personListModel);
    ui->cb_Person->setModelColumn(1);
    m_personMapper->setModel(m_personListModel);
    m_personMapper->addMapping(ui->ed_Telephone, 2);
    m_personMapper->addMapping(ui->ed_Email, 3);
    connect(ui->cb_Person, SIGNAL(currentIndexChanged(int)), this, SLOT(usr_PersonData_changed(int)));

    m_ObjectModel = new QSqlQueryModel(this);
    m_ObjectMapper = new QDataWidgetMapper(this);
    m_ObjectModel->setQuery(prepareQuery(QueryType::Object));
    ui->cb_Object->setModel(m_ObjectModel);
    ui->cb_Object->setModelColumn(1);
    m_ObjectMapper->setModel(m_ObjectModel);
    m_ObjectMapper->addMapping(ui->txt_Adress, 2);
    connect(ui->cb_Object, SIGNAL(currentIndexChanged(int)), this, SLOT(usr_ObjectData_changed(int)));

    m_systemType = new QSqlQueryModel(this);
    m_SystemMapper = new QDataWidgetMapper(this);
    m_systemType->setQuery(prepareQuery(QueryType::SystemType));
    ui->cb_Type->setModel(m_systemType);
    ui->cb_Type->setModelColumn(1);
    m_SystemMapper->setModel(m_systemType);
    connect(ui->cb_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(usr_SystemData_changed(int)));

    ui->cb_Person->setCurrentIndex(-1);
}

QSqlQuery URegisterNewRequest::prepareQuery(URegisterNewRequest::QueryType qType, int index)
{
    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};
    QSqlQuery resultQuery {};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QString textQuery {""};

    switch (qType) {
    case QueryType::SystemType:{
       textQuery = "SELECT * FROM tbl_types ORDER BY id";
       break;
    }
    case QueryType::Person:{
        textQuery = "SELECT * FROM tbl_person ORDER BY name";
        break;
    }
    case QueryType::Object:{
        textQuery = QString("SELECT * FROM tbl_objects WHERE parent_id=%1 ORDER BY name").arg(index);
        break;
    }
    default: textQuery = "";
    }

    std::tie(result, resultQuery) = m_DBProcessor->Execute(textQuery);
    delete m_DBProcessor;
    if (result == DBTypes::DBResult::FAIL) {
        qDebug() << "Error executing DBProcessor";
    }
    return resultQuery;
}

void URegisterNewRequest::on_URegisterNewRequest_rejected()
{
    close();
}

void URegisterNewRequest::on_URegisterNewRequest_accepted()
{
    SaveDialogSettings();
    db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
    int personID;
    int objectID;
    int requestID;
    QVariantList sendArgs {};

    personID = ui->cb_Person->currentIndex();
    if (personID == -1){
        sendArgs << ui->cb_Person->currentText() << ui->ed_Telephone->text() << ui->ed_Email->text();
        personID = m_ReqInserter->AddData( sendArgs, DBTypes::DBInsertType::Person);
    }else {
        personID = m_personListModel->query().value(0).toInt();
    }
    //qDebug() << "Person ID: " << personID;

    objectID = ui->cb_Object->currentIndex();
    if (objectID == -1) {
        sendArgs.clear();
        sendArgs << ui->cb_Object->currentText() << ui->txt_Adress->toPlainText() << personID;
        objectID = m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Object);
    }else {
        objectID = m_ObjectModel->query().value(0).toInt();
    }

    //qDebug() << "Object ID: " << objectID;

    sendArgs.clear();
    sendArgs << objectID << m_systemType->query().value(0).toInt() << ui->textEdit->toPlainText();
    requestID = m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Request);
    //qDebug() << "Request ID: " << requestID;

    m_ReqInserter->UpdateUser(dbUserID, requestID);

    emit db_NewRequest_Add();

    delete  m_ReqInserter;
    close();
}

void URegisterNewRequest::usr_PersonData_changed(int index)
{
    int sendID {m_personListModel->query().value(0).toInt()};
    m_personMapper->setCurrentIndex(index);

    if (index == -1){
        ui->ed_Telephone->setText("");
        ui->ed_Email->setText("");
        ui->cb_Object->setCurrentIndex(-1);
        sendID = index;
    }

    m_ObjectModel->setQuery(prepareQuery(QueryType::Object, sendID));
}

void URegisterNewRequest::usr_ObjectData_changed(int index)
{
    m_ObjectMapper->setCurrentIndex(index);
    if (index == -1) {
        ui->txt_Adress->setText("");
    }
}

void URegisterNewRequest::usr_SystemData_changed(int index)
{
    m_SystemMapper->setCurrentIndex(index);
}

void URegisterNewRequest::on_btn_AddPerson_clicked()
{
    ui->cb_Person->setCurrentIndex(-1);
}

void URegisterNewRequest::on_btn_AddObject_clicked()
{
    ui->cb_Object->setCurrentIndex(-1);
}
