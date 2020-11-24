#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clDBReqInserter.h"
#include "DBProcessor.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mainTableModel (new db::clDBMainQueryModel(this))
{
    ui->setupUi(this);
    RunConnectionDialog(ConnectionDlgMode::StartMode);

    curUser = qgetenv("USER");
    if (curUser.isEmpty()){
        curUser = qgetenv("USERNAME");
    }
    ConfigStatusBar();
    ui->tbl_Requests->setModel(mainTableModel);
    ui->tbl_Requests->hideColumn(12);
    ui->tbl_Requests->setWordWrap(true);
    //ui->tbl_Requests->resizeColumnsToContents();
    ui->tbl_Requests->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tbl_Requests->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tbl_Requests->horizontalHeader()->setStretchLastSection(true);
    ui->tbl_Requests->horizontalHeader()->setSectionsMovable(true);
    ui->tbl_Requests->horizontalHeader()->setVisible(true);
    ui->tbl_Requests->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QObject::connect(ui->tbl_Requests->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex, const QModelIndex)), this, SLOT(usr_ActionsActivity_check(const QModelIndex, const QModelIndex)));
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
    //qDebug() << "NoChanges!!!";
}

void MainWindow::upd_statusBar_dbConnection(bool status)
{
    if (status){
        sts_connection->setText("OK");
    }else{
        sts_connection->setText("DIS");
    }
}

void MainWindow::usr_ActionsActivity_check(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    int statusID {mainTableModel->data(mainTableModel->index(current.row(), 12), Qt::DisplayRole).toInt()};
    if (statusID == 1) {
        ui->act_AcceptRequest->setEnabled(true);
    }else {
        ui->act_AcceptRequest->setEnabled(false);
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
    ui_AddNewWizard = new uiAddNewWizard(dbUserID, this);
    ui_AddNewWizard->setAttribute(Qt::WA_DeleteOnClose, true);
    QObject::connect(ui_AddNewWizard, SIGNAL(usr_NewRequest_added()), this, SLOT(on_act_Refresh_triggered()));
    ui_AddNewWizard->open();
}

void MainWindow::on_act_Refresh_triggered()
{
    mainTableModel->RefreshQuery();
}

void MainWindow::on_act_ReqEdit_triggered()
{
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    ui_editRequest = new UEditRequest(dbUserID, sendID, this);
    ui_editRequest->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_editRequest, SIGNAL(db_RequestUpdate()), this, SLOT(on_act_Refresh_triggered()));
    ui_editRequest->open();
}

void MainWindow::on_act_AcceptRequest_triggered()
{
    ui->act_AcceptRequest->setEnabled(false);
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    QMessageBox engAcceptBox(QMessageBox::Question, "Принять в обработку", QString("Вы действительно хотите принять в обработку заявку №%1").arg(sendID), QMessageBox::Ok | QMessageBox::Cancel, this);
    if (engAcceptBox.exec() == QMessageBox::Ok) {
        db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
        QVariantList sendArgs {};
        sendArgs << 2 << sendID << "Авто: Впервые принят в обработку";
        int lastID {m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Status)};
        if (lastID > -1) {
            qDebug() << "lastID: " << lastID;
            qDebug() << "userI: D" << dbUserID;
            m_ReqInserter->UpdateStatusUser(dbUserID, lastID);
        }
        delete m_ReqInserter;
        on_act_Refresh_triggered();
    }
}
