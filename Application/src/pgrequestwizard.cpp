#include "pgrequestwizard.h"
#include "ui_pgrequestwizard.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QMessageBox>
#include <QDebug>

pgRequestWizard::pgRequestWizard(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pgRequestWizard)
{
    ui->setupUi(this);
    this->setCommitPage(true);
    StartInit();
}

pgRequestWizard::~pgRequestWizard()
{
    delete ui;
}

bool pgRequestWizard::AddRequest()
{
    db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
    QVariantList sendArgs {};

    sendArgs << 0 << 0;
    int costID {m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Cost)};
    if (costID == -1) {
        delete m_ReqInserter;
        QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось добавить заявку", QMessageBox::Ok, this);
        m_MsgBox.exec();
        return false;
    }

    sendArgs.clear();
    sendArgs << objectID << typeID << field("context").toString() << costID;
    int requestID {m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Request)};
    if (requestID == -1) {
        delete m_ReqInserter;
        QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось добавить заявку", QMessageBox::Ok, this);
        m_MsgBox.exec();
        return false;
    }
    if (!(m_ReqInserter->UpdateUser(userID, requestID))) {
        delete m_ReqInserter;
        QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось обновить статус заявки", QMessageBox::Ok, this);
        m_MsgBox.exec();
        return false;
    }
    delete  m_ReqInserter;
    return true;
}

void pgRequestWizard::setUserID(int value)
{
    userID = value;
}

void pgRequestWizard::setObjectID(int value)
{
    objectID = value;
}

void pgRequestWizard::StartInit()
{
    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};
    m_TypeModel = new QSqlQueryModel(this);
    m_TypeModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::SystemType));
    delete m_DBProcessor;

    this->registerField("typeID*", ui->cb_Type);
    this->registerField("context*", ui->txt_Context, "plainText", SIGNAL(textChanged()));
    ui->cb_Type->setModel(m_TypeModel);
    ui->cb_Type->setModelColumn(1);
    ui->cb_Type->setCurrentIndex(-1);
}

void pgRequestWizard::on_cb_Type_currentIndexChanged(int index)
{
    typeID = m_TypeModel->data(m_TypeModel->index(index, 0), Qt::DisplayRole).toInt();
}
