#include "uconnect_db.h"
#include "ui_uconnect_db.h"
#include <QDir>
#include <QString>
#include <QDebug>
#include "settings.h"

UConnect_db::UConnect_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UConnect_db)
{
    ui->setupUi(this);
    GetConnectionSettings();
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

void UConnect_db::GetConnectionSettings()
{
    settings::StoreSettings db_settings;
    db_settings.setGroupName(settings::mapSetGroupNames.at(settings::groupSettings::Connection));
    db_settings.OpenGroup();
    ui->ed_Hostname->setText(db_settings.getParam(settings::mapConSetNames.at(settings::tpConnectNames::Hostname)).toString());
    ui->ed_user->setText(db_settings.getParam(settings::mapConSetNames.at(settings::tpConnectNames::User)).toString());
    ui->cbx_Autoconnect->setChecked(db_settings.getParam(settings::mapConSetNames.at(settings::tpConnectNames::Autoconnect)).toBool());
    if (ui->cbx_Autoconnect->isChecked()){
        ui->ed_Pass->setText(db_settings.getCryptedParam(settings::mapConSetNames.at(settings::tpConnectNames::Password)));
    }
    db_settings.CloseGroup();
}

void UConnect_db::checkAutoconnect()
{
    if (ui->cbx_Autoconnect->isChecked()) this->accept();
}


void UConnect_db::on_UConnect_db_accepted()
{
    settings::StoreSettings db_settings;
    db_settings.setGroupName(settings::mapSetGroupNames.at(settings::groupSettings::Connection));
    db_settings.OpenGroup();
    db_settings.setParam(settings::mapConSetNames.at(settings::tpConnectNames::Hostname), ui->ed_Hostname->text());
    db_settings.setParam(settings::mapConSetNames.at(settings::tpConnectNames::User), ui->ed_user->text());
    db_settings.setCryptedParam(settings::mapConSetNames.at(settings::tpConnectNames::Password), ui->ed_Pass->text());

    if (ui->cbx_Autoconnect->isChecked()){
        db_settings.setParam(settings::mapConSetNames.at(settings::tpConnectNames::Autoconnect), true);
    }else{
        db_settings.setParam(settings::mapConSetNames.at(settings::tpConnectNames::Autoconnect), false);
    }
    db_settings.CloseGroup();
    close();
}
