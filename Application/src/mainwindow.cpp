#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clDBReqInserter.h"
#include "DBProcessor.h"
#include "settings.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mainTableModel (new db::clDBMainQueryModel(this))
{
    ui->setupUi(this);
    LoadDialogSettings();
    RunConnectionDialog(ConnectionDlgMode::StartMode);

    ConfigStatusBar();
    ui->tbl_Requests->setModel(mainTableModel);
    ui->tbl_Requests->horizontalHeader()->setMinimumHeight(ui->tbl_Requests->horizontalHeader()->height() * 2);
    ui->tbl_Requests->setWordWrap(true);
    ui->tbl_Requests->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tbl_Requests->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tbl_Requests->horizontalHeader()->setStretchLastSection(true);
    ui->tbl_Requests->horizontalHeader()->setSectionsMovable(true);
    ui->tbl_Requests->horizontalHeader()->setVisible(true);
    ui->tbl_Requests->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_ActionsList.append(ui->act_Register);
    m_ActionsList.append(ui->act_ReqEdit);
    m_ActionsList.append(ui->act_AcceptRequest);
    m_ActionsList.append(ui->act_ChangeEngineer);
    m_ActionsList.append(ui->act_ChangeStatus);
    m_ActionsList.append(ui->act_SetCost);
    m_ActionsList.append(ui->act_History);
    m_ActionsList.append(ui->act_ReqClose);
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
    sts_accsess = new QLabel();
    sts_username = new QLabel();
    sts_username->setText("");
    sts_accsess->setText("");
    sts_connection->setText("DIS");
    ui->statusbar->addPermanentWidget(sts_username);
    ui->statusbar->addPermanentWidget(sts_accsess);
    ui->statusbar->addPermanentWidget(sts_connection);
}

void MainWindow::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("MainWindow");
    m_settings->OpenGroup();
    m_settings->setParam("Geometry", this->saveGeometry());
    m_settings->setParam("MainTableGeometry", ui->tbl_Requests->horizontalHeader()->saveState());
    m_settings->CloseGroup();
    delete m_settings;
}

void MainWindow::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("MainWindow");
    m_settings->OpenGroup();
    this->restoreGeometry(m_settings->getParam("Geometry").toByteArray());
    ui->tbl_Requests->horizontalHeader()->restoreState(m_settings->getParam("MainTableGeometry").toByteArray());
    m_settings->CloseGroup();
    delete m_settings;
}

void MainWindow::getColumnsEnabled()
{
    m_ColumnViewActions.clear();
    for (int i = 0; i < m_AccessLevel->getColumnCount(); i++) {
        if (m_AccessLevel->getColumnsList()->value(i)) {
            QAction *addAction = ui->menu_Columns->addAction(mainTableModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
            addAction->setVisible(true);
            addAction->setCheckable(true);
            m_ColumnViewActions.append(addAction);
        }else {
            ui->tbl_Requests->setColumnHidden(i, true);
        }
    }
}

void MainWindow::getActionsEnabled()
{
    for (int i = 0; i < m_AccessLevel->getActionsCount(); i++) {
        m_ActionsList.value(i)->setEnabled(m_AccessLevel->getActionAccessList()->value(i));
        m_ActionsList.value(i)->setVisible(m_AccessLevel->getActionAccessList()->value(i));
    }
}

void MainWindow::ConnectToDB()
{
    QString curUser = qgetenv("USER");
    if (curUser.isEmpty()){
        curUser = qgetenv("USERNAME");
    }
    db::DBProcessor* m_DBProcessor {new db::DBProcessor()};
    std::tie(dbUserID, dbUserName) = m_DBProcessor->findUser(curUser);
    sts_username->setText(dbUserName);
    delete m_DBProcessor;

    if (m_AccessLevel == nullptr) delete m_AccessLevel;
    m_AccessLevel = new db::clDBAccessLevel(dbUserID, this);
    sts_accsess->setText(m_AccessLevel->getAccessName());
    getActionsEnabled();
    QObject::connect(ui->tbl_Requests->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex, const QModelIndex)), this, SLOT(usr_ActionsActivity_check(const QModelIndex, const QModelIndex)));
    getColumnsEnabled();
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
    int statusID {mainTableModel->data(mainTableModel->index(current.row(), 13), Qt::DisplayRole).toInt()};
    if (m_AccessLevel->getActionAccessList()->value(2)) {
        if (statusID == 1) {
            ui->act_AcceptRequest->setEnabled(true);
        }else {
            ui->act_AcceptRequest->setEnabled(false);
        }
    }

    if (m_AccessLevel->getActionAccessList()->value(4)) {
        if ((statusID > 1) && (statusID < 7)) {
            ui->act_ChangeStatus->setEnabled(true);
        }else {
            ui->act_ChangeStatus->setEnabled(false);
        }
    }

    if (m_AccessLevel->getActionAccessList()->value(5)) {
        if (statusID > 6) {
            ui->act_SetCost->setEnabled(false);
        }else {
            ui->act_SetCost->setEnabled(true);
        }
    }

    if (m_AccessLevel->getActionAccessList()->value(7)) {
        if (statusID > 6) {
            ui->act_ReqClose->setEnabled(false);
        }else {
            ui->act_ReqClose->setEnabled(true);
        }
    }
}

void MainWindow::on_act_DBConnection_triggered()
{
    RunConnectionDialog(ConnectionDlgMode::RunMode);
}

void MainWindow::on_act_Exit_triggered()
{
    SaveDialogSettings();
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
    QModelIndex cID {ui->tbl_Requests->selectionModel()->currentIndex()};
    mainTableModel->RefreshQuery();
    ui->tbl_Requests->selectionModel()->setCurrentIndex(cID,QItemSelectionModel::Rows);
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
    QMessageBox engAcceptBox(QMessageBox::Question, "Принять в обработку", QString("Вы действительно хотите принять в обработку заявку №%1?").arg(sendID), QMessageBox::Ok | QMessageBox::Cancel, this);
    if (engAcceptBox.exec() == QMessageBox::Ok) {
        db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
        QVariantList sendArgs {};
        sendArgs << 2 << sendID << "Авто: Впервые принят в обработку" << 1;
        int lastID {m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Status)};
        if (lastID == -1) {
            QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось изменить статус заявки", QMessageBox::Ok, this);
            m_MsgBox.exec();
        }else {
            if (!(m_ReqInserter->UpdateUser(dbUserID, sendID))) {
                QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось обновить статус", QMessageBox::Ok, this);
                m_MsgBox.exec();
            }
        }
        delete m_ReqInserter;
        on_act_Refresh_triggered();
    }
}

void MainWindow::on_act_ChangeEngineer_triggered()
{
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    ui_ChooseEngineer = new uiChooseEngineer(std::make_pair(sendID, dbUserID), this);
    ui_ChooseEngineer->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_ChooseEngineer, SIGNAL(usr_Engineer_update()), this, SLOT(on_act_Refresh_triggered()));
    ui_ChooseEngineer->open();
}

void MainWindow::on_act_ChangeStatus_triggered()
{
    //ui->act_ChangeStatus->setEnabled(false);
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    int stateID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 13), Qt::DisplayRole).toInt()};
    ui_ChangeStatus = new uiChangeStatus(std::make_pair(sendID, dbUserID), stateID, this);
    ui_ChangeStatus->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_ChangeStatus, SIGNAL(usr_ReqStatus_update()), this, SLOT(on_act_Refresh_triggered()));
    ui_ChangeStatus->open();
}

void MainWindow::on_act_ReqClose_triggered()
{
    //ui->act_ReqClose->setEnabled(false);
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    ui_ChangeStatus = new uiChangeStatus(std::make_pair(sendID, dbUserID), 6, this);
    ui_ChangeStatus->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_ChangeStatus, SIGNAL(usr_ReqStatus_update()), this, SLOT(on_act_Refresh_triggered()));
    ui_ChangeStatus->open();
}

void MainWindow::on_act_SetCost_triggered()
{
    //ui->act_SetCost->setEnabled(false);
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 14), Qt::DisplayRole).toInt()};
    ui_SetCost = new uiSetCost(std::make_pair(sendID, dbUserID), this);
    ui_SetCost->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_SetCost, SIGNAL(usr_ReqCost_update()), this, SLOT(on_act_Refresh_triggered()));
    ui_SetCost->open();
}

void MainWindow::on_act_History_triggered()
{
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    ui_ProtocolView = new uiProtocolView(sendID, this);
    ui_ProtocolView->setAttribute(Qt::WA_DeleteOnClose);
    ui_ProtocolView->open();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    SaveDialogSettings();
    event->accept();
}

void MainWindow::on_act_AboutQT_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_act_About_triggered()
{
    QMessageBox::about(this, "Информация о программе", "Программа для работы с заявками. \nСалон Охранных Систем");
}
