#include "uiaddnewwizard.h"
#include "ui_uiaddnewwizard.h"
#include "DBProcessor.h"
#include <QPixmap>
#include <QWizardPage>
#include <QDebug>

uiAddNewWizard::uiAddNewWizard(int userID, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::uiAddNewWizard),
    dbUserID(userID)
{
    personPage = new pgPersonWizard(this);
    this->addPage(personPage);
    ui->setupUi(this);
    this->setButtonText(QWizard::NextButton, "Далее");
    this->setButtonText(QWizard::CancelButton, "Отмена");
}

uiAddNewWizard::~uiAddNewWizard()
{
    delete ui;
}


void uiAddNewWizard::SaveDialogSettings()
{

}

void uiAddNewWizard::LoadDialogSettings()
{

}
