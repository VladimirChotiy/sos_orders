#include "chooseengineer.h"
#include "ui_chooseengineer.h"
#include <QSettings>
#include <QDir>


ChooseEngineer::ChooseEngineer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseEngineer)
{
    ui->setupUi(this);
    LoadDialogSettings();
}

ChooseEngineer::~ChooseEngineer()
{
    delete ui;
}

void ChooseEngineer::LoadDialogSettings()
{
    QSettings dlg_settings(QDir::currentPath() + "/settings.cfg", QSettings::IniFormat);
    dlg_settings.beginGroup("choose_eng_dlg");
    this->resize(dlg_settings.value("width", "250").toInt(), dlg_settings.value("heigth", "125").toInt());
    dlg_settings.endGroup();
}

void ChooseEngineer::SaveDialogSettings()
{
    QSettings dlg_settings(QDir::currentPath() + "/settings.cfg", QSettings::IniFormat);
    dlg_settings.beginGroup("choose_eng_dlg");
    dlg_settings.setValue("width", this->geometry().width());
    dlg_settings.setValue("heigth", this->geometry().height());
    dlg_settings.endGroup();
}

void ChooseEngineer::on_ChooseEngineer_rejected()
{
    SaveDialogSettings();
    close();
}

void ChooseEngineer::on_ChooseEngineer_accepted()
{
    SaveDialogSettings();
    close();
}
