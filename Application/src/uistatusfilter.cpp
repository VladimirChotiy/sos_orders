#include "uistatusfilter.h"
#include "ui_uistatusfilter.h"

uiStatusFilter::uiStatusFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiStatusFilter)
{
    ui->setupUi(this);
}

uiStatusFilter::~uiStatusFilter()
{
    delete ui;
}

void uiStatusFilter::on_uiStatusFilter_accepted()
{

}

void uiStatusFilter::on_uiStatusFilter_rejected()
{

}
