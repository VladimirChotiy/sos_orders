#include "uregisternewrequest.h"
#include "ui_uregisternewrequest.h"
#include "DBProcessor.h"
#include "dbtypes.h"
#include "settings.h"
#include <QSettings>
#include <QDir>

URegisterNewRequest::URegisterNewRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URegisterNewRequest)
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
    m_settings->setParam("width", this->geometry().width());
    m_settings->setParam("heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void URegisterNewRequest::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("AddNewRequestDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("width").toInt(), m_settings->getParam("heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void URegisterNewRequest::StartInit()
{
    ui->setupUi(this);
    ui->ed_Date->setDate(QDate::currentDate());
    LoadDialogSettings();

    m_personListModel = new QSqlTableModel(this);
    m_personListModel->setTable("tbl_person");
    m_personListModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_personListModel->setSort(1, Qt::SortOrder::DescendingOrder);
    m_personListModel->select();
    m_personMapper = new QDataWidgetMapper(this);
    ui->cb_Person->setModel(m_personListModel);
    ui->cb_Person->setModelColumn(1);
    m_personMapper->setModel(m_personListModel);
    m_personMapper->addMapping(ui->ed_Telephone, 2);
    m_personMapper->addMapping(ui->ed_Email, 3);
    connect(ui->cb_Person, SIGNAL(currentIndexChanged(int)), this, SLOT(usr_PersonData_changed(int)));

    m_ObjectModel = new QSqlQueryModel(this);
    m_ObjectMapper = new QDataWidgetMapper(this);
    m_ObjectModel->setQuery(QSqlQuery(QString("SELECT * FROM tbl_objects WHERE parent_id=-1 ORDER BY name")));
    ui->cb_Object->setModel(m_ObjectModel);
    ui->cb_Object->setModelColumn(1);
    m_ObjectMapper->setModel(m_ObjectModel);
    m_ObjectMapper->addMapping(ui->txt_Adress, 2);
    connect(ui->cb_Object, SIGNAL(currentIndexChanged(int)), m_ObjectMapper, SLOT(setCurrentIndex(int)));

    m_systemType = new QSqlTableModel(this);
    m_systemType->setTable("tbl_types");
    m_systemType->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_systemType->setSort(0, Qt::SortOrder::DescendingOrder);
    m_systemType->select();
    ui->cb_Type->setModel(m_systemType);
    ui->cb_Type->setModelColumn(1);

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
    }
    default: textQuery = "";
    }

    std::tie(result, resultQuery) = m_DBProcessor->Execute(textQuery);
    if (result == DBTypes::DBResult::FAIL) {
        qDebug() << "Error executing DBProcessor";
    }
    return resultQuery;
}

void URegisterNewRequest::on_URegisterNewRequest_rejected()
{
    SaveDialogSettings();
    close();
}

void URegisterNewRequest::on_URegisterNewRequest_accepted()
{
    SaveDialogSettings();
    close();
}

void URegisterNewRequest::usr_PersonData_changed(int index)
{
    m_personMapper->setCurrentIndex(index);

    m_ObjectModel->setQuery(QString("SELECT * FROM tbl_objects WHERE parent_id=%1 ORDER BY name").arg(index));
}

void URegisterNewRequest::on_btn_AddPerson_clicked()
{
    ui->cb_Person->setCurrentIndex(-1);
    ui->ed_Telephone->setText("");
    ui->ed_Email->setText("");
}

void URegisterNewRequest::on_btn_AddObject_clicked()
{
    ui->cb_Object->setCurrentIndex(-1);
    ui->txt_Adress->setText("");
}
