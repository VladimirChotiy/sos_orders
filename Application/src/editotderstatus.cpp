#include "editotderstatus.h"
#include "ui_editotderstatus.h"
#include <QSettings>
#include <QDir>

EditOtderStatus::EditOtderStatus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOtderStatus)
{
    ui->setupUi(this);
    LoadDialogSettings();
}

EditOtderStatus::~EditOtderStatus()
{
    delete ui;
}

void EditOtderStatus::on_EditOtderStatus_accepted()
{
    SaveDialogSettings();
    close();
}

void EditOtderStatus::on_EditOtderStatus_rejected()
{
    SaveDialogSettings();
    close();
}

void EditOtderStatus::SaveDialogSettings()
{
    QSettings dlg_settings(QDir::currentPath() + "/settings.cfg", QSettings::IniFormat);
    dlg_settings.beginGroup("edit_request_dlg");
    dlg_settings.setValue("width", this->geometry().width());
    dlg_settings.setValue("heigth", this->geometry().height());
    dlg_settings.endGroup();
}

void EditOtderStatus::LoadDialogSettings()
{
    QSettings dlg_settings(QDir::currentPath() + "/settings.cfg", QSettings::IniFormat);
    dlg_settings.beginGroup("edit_request_dlg");
    this->resize(dlg_settings.value("width", "600").toInt(), dlg_settings.value("heigth", "425").toInt());
    dlg_settings.endGroup();
}
