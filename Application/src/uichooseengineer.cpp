#include "uichooseengineer.h"
#include "ui_uichooseengineer.h"
#include "DBProcessor.h"
#include "settings.h"
#include "clDBReqInserter.h"
#include <QMessageBox>

uiChooseEngineer::uiChooseEngineer(std::pair<int, int> indexes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiChooseEngineer)
{
    ui->setupUi(this);
    std::tie(reqID, userID) = indexes;
    LoadDialogSettings();
    StartInit();
}

uiChooseEngineer::~uiChooseEngineer()
{
    delete ui;
}

void uiChooseEngineer::StartInit()
{
    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};
    m_EngineerModel = new QSqlQueryModel(this);

    m_EngineerModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Engineer, 2));
    ui->cb_User->setModel(m_EngineerModel);
    ui->cb_User->setModelColumn(2);
    ui->cb_User->setCurrentIndex(-1);

    delete m_DBProcessor;
}

void uiChooseEngineer::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("ChangeEngineerDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiChooseEngineer::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("ChangeEngineerDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiChooseEngineer::ChangeEng()
{
    db::clDBReqInserter *m_DBReqInserter {new db::clDBReqInserter(this)};
    QVariantList sendArgs {};
    QString txtComment {ui->txt_Comment->toPlainText()};
    int result;

    if (txtComment == "") txtComment = "Авто: Ответвтенный за заявку инженер был изменен";
    sendArgs << 10 << reqID << txtComment << userID;
    result = m_DBReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Status);
    if (result == -1) {
        QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось изменить инженера", QMessageBox::Ok, this);
        m_MsgBox.exec();
    }else {
        sendArgs.clear();
        sendArgs << m_EngineerModel->data(m_EngineerModel->index(ui->cb_User->currentIndex(), 0), Qt::DisplayRole).toInt();
        m_DBReqInserter->UpdateData(reqID, sendArgs, DBTypes::DBUpdateType::Engineer);
        emit usr_Engineer_update();
    }

    delete m_DBReqInserter;
}

void uiChooseEngineer::on_uiChooseEngineer_accepted()
{
    SaveDialogSettings();
    ChangeEng();
    close();
}

void uiChooseEngineer::on_uiChooseEngineer_rejected()
{
    close();
}
