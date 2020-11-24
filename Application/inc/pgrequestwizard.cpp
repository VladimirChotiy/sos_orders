#include "pgrequestwizard.h"
#include "ui_pgrequestwizard.h"

pgRequestWizard::pgRequestWizard(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pgRequestWizard)
{
    ui->setupUi(this);
}

pgRequestWizard::~pgRequestWizard()
{
    delete ui;
}
