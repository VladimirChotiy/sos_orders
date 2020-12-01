#include "uiprotocolview.h"
#include "ui_uiprotocolview.h"
#include "settings.h"
#include <QSqlQuery>
#include <QDebug>

uiProtocolView::uiProtocolView(int index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiProtocolView),
    reqID(index)
{
    ui->setupUi(this);
    LoadDialogSettings();
    StartInit();
    QObject::connect(ui->tbl_Protocol->horizontalHeader(), &QHeaderView::sectionResized, ui->tbl_Protocol, &QTableView::resizeRowsToContents);
}

uiProtocolView::~uiProtocolView()
{
    delete ui;
}

void uiProtocolView::StartInit()
{
    m_ProtocolModel = new db::clDBProtocolQueryModel(reqID, this);
    ui->tbl_Protocol->setModel(m_ProtocolModel);
    ui->tbl_Protocol->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tbl_Protocol->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tbl_Protocol->horizontalHeader()->setStretchLastSection(true);
    ui->tbl_Protocol->horizontalHeader()->setSectionsMovable(true);
    ui->tbl_Protocol->horizontalHeader()->setVisible(true);
    ui->tbl_Protocol->horizontalHeader()->setMinimumHeight(ui->tbl_Protocol->horizontalHeader()->height() * 2);
}

void uiProtocolView::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("ProtocolViewDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->setParam("TblGeometry", ui->tbl_Protocol->horizontalHeader()->saveState());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiProtocolView::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("ProtocolViewDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    ui->tbl_Protocol->horizontalHeader()->restoreState(m_settings->getParam("TblGeometry").toByteArray());
    ui->tbl_Protocol->resizeRowsToContents();
    m_settings->CloseGroup();
    delete m_settings;
}

void uiProtocolView::on_uiProtocolView_accepted()
{
    SaveDialogSettings();
    close();
}
