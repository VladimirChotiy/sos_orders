#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DBProcessor.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mainTableModel (new db::clDBMainQueryModel(0, this))
{
    ui->setupUi(this);
    RunConnectionDialog(ConnectionDlgMode::StartMode);

    curUser = qgetenv("USER");
    if (curUser.isEmpty()){
        curUser = qgetenv("USERNAME");
    }
    ConfigStatusBar();
    ui->tbl_Requests->setModel(mainTableModel);
    ui->tbl_Requests->setWordWrap(true);
    //ui->tbl_Requests->resizeColumnsToContents();
    ui->tbl_Requests->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tbl_Requests->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tbl_Requests->horizontalHeader()->setStretchLastSection(true);
    ui->tbl_Requests->horizontalHeader()->setSectionsMovable(true);
    ui->tbl_Requests->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
    ui_editOrderStatus = new EditOtderStatus(this);
    ui_editOrderStatus->open();
}

void MainWindow::ConnectToDB()
{
    db::DBProcessor* m_DBProcessor {new db::DBProcessor()};
    std::tie(dbUserID, dbUserName) = m_DBProcessor->findUser(curUser);
    sts_username->setText(dbUserName);
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
    URegisterRequest = new URegisterNewRequest(dbUserID, this);
    connect(URegisterRequest, SIGNAL(db_NewRequest_Add()), this, SLOT(on_act_Refresh_triggered()));
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

void MainWindow::on_act_Refresh_triggered()
{
    mainTableModel->RefreshQuery();
}

void MainWindow::on_pushButton_clicked()
{
    ui_editRequest = new UEditRequest(this);
    ui_editRequest->open();
}
