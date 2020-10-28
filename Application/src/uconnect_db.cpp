#include "uconnect_db.h"
#include "ui_uconnect_db.h"
#include <QDir>
#include <QString>
#include <QDebug>

UConnect_db::UConnect_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UConnect_db)
{
    ui->setupUi(this);
    LoadDialogUiSettings();
    LoadConnectionSettings();
    tmr_Autoconnect = new QTimer();
    tmr_Autoconnect->setSingleShot(true);
    tmr_Autoconnect->start(500);
    QObject::connect(tmr_Autoconnect, SIGNAL(timeout()), this, SIGNAL(timerShot()));
}

UConnect_db::~UConnect_db()
{
    delete ui;
}

void UConnect_db::on_UConnect_db_rejected()
{
    close();
}

void UConnect_db::LoadConnectionSettings()
{
    settings::StoreSettings* db_settings {new settings::StoreSettings()};
    db_settings->setGroupName("Connection");
    db_settings->OpenGroup();
    ui->ed_Hostname->setText(db_settings->getParam("Hostname").toString());
    ui->ed_user->setText(db_settings->getParam("User").toString());
    ui->cbx_Autoconnect->setChecked(db_settings->getParam("Autoconnect").toBool());
    if (ui->cbx_Autoconnect->isChecked()){
        ui->ed_Pass->setText(db_settings->getCryptedParam("Password"));
    }
    db_settings->CloseGroup();
    delete  db_settings;
}

void UConnect_db::SaveConnectionSettings()
{
    settings::StoreSettings* db_settings {new settings::StoreSettings()};
    db_settings->setGroupName("Connection");
    db_settings->OpenGroup();
    db_settings->setParam("Hostname", ui->ed_Hostname->text());
    db_settings->setParam("User", ui->ed_user->text());
    db_settings->setCryptedParam("Password", ui->ed_Pass->text());
    db_settings->setParam("Autoconnect", ui->cbx_Autoconnect->isChecked());
    db_settings->CloseGroup();
    delete db_settings;
}

void UConnect_db::LoadDialogUiSettings()
{
    settings::StoreSettings* ui_settings {new settings::StoreSettings()};
    ui_settings->setGroupName("ConnectDialog");
    ui_settings->OpenGroup();
    QSize uiSize {ui_settings->getParam("Width").toInt(), ui_settings->getParam("Height").toInt()};
    if (uiSize.isEmpty()) {
        uiSize.setWidth(432);
        uiSize.setHeight(157);
    }
    this->resize(uiSize);
    ui_settings->CloseGroup();
    delete  ui_settings;
}

void UConnect_db::SaveDialogUiSettings()
{
    settings::StoreSettings* ui_settings {new settings::StoreSettings()};
    ui_settings->setGroupName("ConnectDialog");
    ui_settings->OpenGroup();
    ui_settings->setParam("Width", this->geometry().width());
    ui_settings->setParam("Height", this->geometry().height());
    ui_settings->CloseGroup();
    delete  ui_settings;
}

void UConnect_db::checkAutoconnect()
{
    if (ui->cbx_Autoconnect->isChecked()) this->accept();
}


void UConnect_db::on_UConnect_db_accepted()
{
    SaveConnectionSettings();
    SaveDialogUiSettings();
    close();
}
