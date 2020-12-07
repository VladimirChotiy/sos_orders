#include "uichangestatus.h"
#include "ui_uichangestatus.h"
#include "settings.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QMessageBox>

uiChangeStatus::uiChangeStatus(std::pair<int, int> indexes, int state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiChangeStatus),
    curState(state)
{
    ui->setupUi(this);
    std::tie(reqID, userID) = indexes;
    LoadDialogSettings();
    StartInit();
}

uiChangeStatus::~uiChangeStatus()
{
    delete ui;
}

void uiChangeStatus::StartInit()
{
    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};
    m_StatusModel = new QSqlQueryModel(this);
    m_StatusModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Status, curState));
    ui->cb_Status->setModel(m_StatusModel);
    ui->cb_Status->setModelColumn(1);
    ui->cb_Status->setCurrentIndex(-1);

}

void uiChangeStatus::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("ChangeStatusDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiChangeStatus::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("ChangeStatusDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiChangeStatus::ChangeStatus()
{
    db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
    QVariantList sendArgs {};
    int result;

    int stID {m_StatusModel->data(m_StatusModel->index(ui->cb_Status->currentIndex(), 0), Qt::DisplayRole).toInt()};
    sendArgs << stID << reqID << ui->txt_Comment->toPlainText() << 1;
    result = m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Status);
    if (result == -1) {
        QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось изменить инженера", QMessageBox::Ok, this);
        m_MsgBox.exec();
    }else {
        m_ReqInserter->UpdateUser(userID, reqID);
        emit usr_ReqStatus_update();
    }
    delete m_ReqInserter;
}

void uiChangeStatus::on_uiChangeStatus_accepted()
{
    SaveDialogSettings();
    ChangeStatus();
    close();
}

void uiChangeStatus::on_uiChangeStatus_rejected()
{
    close();
}
