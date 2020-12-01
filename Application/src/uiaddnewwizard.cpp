#include "uiaddnewwizard.h"
#include "ui_uiaddnewwizard.h"
#include "DBProcessor.h"
#include "settings.h"
#include <QPixmap>
#include <QWizardPage>
#include <QDebug>

uiAddNewWizard::uiAddNewWizard(int userID, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::uiAddNewWizard),
    dbUserID(userID)
{
    personPage = new pgPersonWizard(this);
    objectPage = new pgObjectWizard(this);
    requestPage = new pgRequestWizard(this);
    this->addPage(personPage);
    this->addPage(objectPage);
    this->addPage(requestPage);
    ui->setupUi(this);
    LoadDialogSettings();
    this->setButtonText(QWizard::NextButton, "Далее");
    this->setButtonText(QWizard::CancelButton, "Отмена");
    this->setButtonText(QWizard::BackButton, "Назад");
    this->setButtonText(QWizard::FinishButton, "Завершить");
    requestPage->setUserID(userID);
    QWizard::setDefaultProperty("QTextEdit", "plainText", SIGNAL(textChanged()));
    QObject::connect(personPage, SIGNAL(usr_PersonId_changed(int)), objectPage, SLOT(setPersonID(int)));
    QObject::connect(objectPage, SIGNAL(usr_ObjectId_changed(int)), requestPage, SLOT(setObjectID(int)));
}

uiAddNewWizard::~uiAddNewWizard()
{
    delete ui;
}


void uiAddNewWizard::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("NewRequestWizard");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiAddNewWizard::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("NewRequestWizard");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiAddNewWizard::on_uiAddNewWizard_currentIdChanged(int id)
{
    switch (id) {
    case 1: {
        objectPage->RefreshObjectPage();
        break;
    }
    }
}

void uiAddNewWizard::on_uiAddNewWizard_rejected()
{
    SaveDialogSettings();
    close();
}

void uiAddNewWizard::on_uiAddNewWizard_accepted()
{
    SaveDialogSettings();
    if (requestPage->AddRequest()) emit usr_NewRequest_added();
    close();
}
