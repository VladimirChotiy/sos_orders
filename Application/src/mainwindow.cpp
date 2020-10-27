#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DBProcessor.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RunConnectionDialog(ConnectionDlgMode::StartMode);

    curUser = qgetenv("USER");
    if (curUser.isEmpty()){
        curUser = qgetenv("USERNAME");
    }
    ConfigStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RunConnectionDialog(ConnectionDlgMode mode)
{
    ui_connect = new UConnect_db(this);
    ui_connect->open();
    QObject::connect(ui_connect, SIGNAL(accepted()), this, SLOT(ConnectToDB()));
    if(mode == ConnectionDlgMode::StartMode){
        QObject::connect(ui_connect, SIGNAL(rejected()), this, SLOT(close()));
        QObject::connect(ui_connect, SIGNAL(timerShot()), ui_connect, SLOT(checkAutoconnect()));
    }else if (mode == ConnectionDlgMode::RunMode){
        QObject::connect(ui_connect, SIGNAL(rejected()), this, SLOT(NoChangesConnectionDlg()));
    }

}

void MainWindow::ConfigStatusBar()
{
    sts_connection = new QLabel();
    sts_username = new QLabel();
    sts_username->setText("");
    sts_connection->setText("DIS");
    ui->statusbar->addPermanentWidget(sts_username);
    ui->statusbar->addPermanentWidget(sts_connection);
}

void MainWindow::RunEditDlg()
{
    ui_editRequest = new EditOtderStatus(this);
    ui_editRequest->open();
}

void MainWindow::ConnectToDB()
{
    db::DBProcessor* m_DBProcessor {new db::DBProcessor()};
    sts_username->setText(m_DBProcessor->findUser(curUser));

    delete m_DBProcessor;
}

void MainWindow::NoChangesConnectionDlg()
{
    qDebug() << "NoChanges!!!";
}

void MainWindow::upd_statusBar_dbConnection(bool status)
{
    if (status){
        sts_connection->setText("OK");
    }else{
        sts_connection->setText("DIS");
    }
}


void MainWindow::on_act_DBConnection_triggered()
{
    RunConnectionDialog(ConnectionDlgMode::RunMode);
}

void MainWindow::on_act_Exit_triggered()
{
    close();
}


void MainWindow::on_act_Register_triggered()
{
    URegisterRequest = new URegisterNewRequest(this);
    URegisterRequest->open();
}

void MainWindow::on_act_Accept_triggered()
{
    ui_chooseEng = new ChooseEngineer(this);
    ui_chooseEng->open();
}

void MainWindow::on_act_Survey_triggered()
{
    RunEditDlg();
}
