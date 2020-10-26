#include "uregisternewrequest.h"
#include "ui_uregisternewrequest.h"
#include <QSettings>
#include <QDir>

URegisterNewRequest::URegisterNewRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URegisterNewRequest)
{
    ui->setupUi(this);
    ui->ed_Date->setDate(QDate::currentDate());
    LoadDialogSettings();
    on_btn_Card_clicked();
    on_btn_ObjectAddress_clicked();
}

URegisterNewRequest::~URegisterNewRequest()
{
    delete ui;
}

void URegisterNewRequest::on_btn_Card_clicked()
{
    if (ui->btn_Card->isChecked()){
        ui->lbl_Telephone->show();
        ui->ed_Telephone->show();
        ui->lbl_Email->show();
        ui->ed_Email->show();
        ui->gbox_Person->setMinimumHeight(202);
    }else{
        ui->lbl_Telephone->hide();
        ui->ed_Telephone->hide();
        ui->lbl_Email->hide();
        ui->ed_Email->hide();
        ui->gbox_Person->setMinimumHeight(101);
    }
}

void URegisterNewRequest::on_btn_ObjectAddress_clicked()
{
    if (ui->btn_ObjectAddress->isChecked()){
        ui->lbl_Address->show();
        ui->txt_Adress->show();
        ui->gbox_Object->setMinimumHeight(161);
    }else{
        ui->lbl_Address->hide();
        ui->txt_Adress->hide();
        ui->gbox_Object->setMinimumHeight(111);
    }
}

void URegisterNewRequest::SaveDialogSettings()
{
    QSettings dlg_settings(QDir::currentPath() + "/settings.cfg", QSettings::IniFormat);
    dlg_settings.beginGroup("new_request_dlg");
    dlg_settings.setValue("width", this->geometry().width());
    dlg_settings.setValue("heigth", this->geometry().height());
    dlg_settings.setValue("btnCard_checked", ui->btn_Card->isChecked());
    dlg_settings.setValue("btnAddEdit_checked", ui->btn_AddEdit->isChecked());
    dlg_settings.endGroup();
}

void URegisterNewRequest::LoadDialogSettings()
{
    QSettings dlg_settings(QDir::currentPath() + "/settings.cfg", QSettings::IniFormat);
    dlg_settings.beginGroup("new_request_dlg");
    this->resize(dlg_settings.value("width", "600").toInt(), dlg_settings.value("heigth", "425").toInt());
    ui->btn_Card->setChecked(dlg_settings.value("btnCard_checked", "true").toBool());
    ui->btn_AddEdit->setChecked(dlg_settings.value("btnAddEdit_checked", "true").toBool());
    dlg_settings.endGroup();
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
