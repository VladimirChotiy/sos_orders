#include "uichooseengineer.h"
#include "ui_uichooseengineer.h"
#include <QDateEdit>

uiChooseEngineer::uiChooseEngineer(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiChooseEngineer),
    reqID(id)
{
    ui->setupUi(this);
QDateEdit();
}

uiChooseEngineer::~uiChooseEngineer()
{
    delete ui;
}

void uiChooseEngineer::StartInit()
{

}
