#include "uisetcost.h"
#include "ui_uisetcost.h"
#include "settings.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QSqlQueryModel>
#include <QAbstractButton>
#include <QMessageBox>
#include <QDebug>

uiSetCost::uiSetCost(std::pair<int, int> indexes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiSetCost)
{
    ui->setupUi(this);
    std::tie(reqID, userID) = indexes;
    LoadDialogSettings();
    StartInit();
}

uiSetCost::~uiSetCost()
{
    delete ui;
}

void uiSetCost::on_uiSetCost_accepted()
{
    SaveDialogSettings();
    ChangeCost();
    close();
}

void uiSetCost::on_uiSetCost_rejected()
{
    close();
}

void uiSetCost::StartInit()
{
    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};
    QSqlQueryModel *m_CostModel = new QSqlQueryModel(this);
    m_CostModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Cost, reqID));
    ui->sb_mCost->setValue(m_CostModel->data(m_CostModel->index(0,1), Qt::DisplayRole).toDouble());
    ui->sb_wCost->setValue(m_CostModel->data(m_CostModel->index(0,2), Qt::DisplayRole).toDouble());
    delete  m_CostModel;
    delete  m_DBProcessor;
}

void uiSetCost::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("SetCostDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiSetCost::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("SetCostDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiSetCost::ChangeCost()
{
    db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
    QVariantList sendArgs {};
    sendArgs << ui->sb_mCost->value() << ui->sb_wCost->value();

    m_ReqInserter->UpdateData(reqID, sendArgs, DBTypes::DBUpdateType::Cost);
    sendArgs.clear();

    sendArgs << 11 << reqID << "Авто: Изменена стоимость" << userID;
    m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Status);

    delete m_ReqInserter;
}

void uiSetCost::on_bb_Dialog_clicked(QAbstractButton *button)
{
    if (ui->bb_Dialog->buttonRole(button) == QDialogButtonBox::ResetRole) {
        StartInit();
    }
}
