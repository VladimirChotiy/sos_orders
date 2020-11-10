#include "ueditrequest.h"
#include "ui_ueditrequest.h"

UEditRequest::UEditRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ueditrequest)
{
    ui->setupUi(this);
}

UEditRequest::~UEditRequest()
{
    delete ui;
}
void UEditRequest::on_ueditrequest_accepted()
{
    close();
}

void UEditRequest::on_ueditrequest_rejected()
{
    close();
}
