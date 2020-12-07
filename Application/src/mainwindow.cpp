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
    ui->tbl_Requests->setVisible(false);
    ui->tbl_Requests->setModel(mainTableModel);
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

    m_ColumnsActions.append(ui->act_ColNumber);
    m_ColumnsActions.append(ui->act_ColContext);
    m_ColumnsActions.append(ui->act_ColComment);
    m_ColumnsActions.append(ui->act_ColType);
    m_ColumnsActions.append(ui->act_ColObject);
    m_ColumnsActions.append(ui->act_ColAddress);
    m_ColumnsActions.append(ui->act_ColPerson);
    m_ColumnsActions.append(ui->act_ColTel);
    m_ColumnsActions.append(ui->act_ColEmail);
    m_ColumnsActions.append(ui->act_ColData);
    m_ColumnsActions.append(ui->act_ColStatus);
    m_ColumnsActions.append(ui->act_ColUser);
    m_ColumnsActions.append(ui->act_ColEng);
    m_ColumnsActions.append(ui->act_ColMCost);
    m_ColumnsActions.append(ui->act_ColWCost);
    m_ColumnsActions.append(ui->act_ColSum);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RunConnectionDialog(ConnectionDlgMode mode)
{
    ui_connect = new UConnect_db(this);
    ui_connect->setAttribute(Qt::WA_DeleteOnClose);
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
    m_settings->setParam("MainToolBar", ui->toolBarMain->isVisible());
    m_settings->setParam("RequestToolBar", ui->toolBarOrder->isVisible());
    m_settings->CloseGroup();
    delete m_settings;

    SaveTableSettings();
}

void MainWindow::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("MainWindow");
    m_settings->OpenGroup();
    this->restoreGeometry(m_settings->getParam("Geometry").toByteArray());
    ui->toolBarMain->setVisible(m_settings->getParam("MainToolBar").toBool());
    ui->toolBarOrder->setVisible(m_settings->getParam("RequestToolBar").toBool());
    m_settings->CloseGroup();
    delete m_settings;
}

void MainWindow::SaveTableSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("MainWindow");
    m_settings->OpenGroup();
    m_settings->setParam("MainTableGeometry", ui->tbl_Requests->horizontalHeader()->saveState());
    m_settings->CloseGroup();
    delete m_settings;
}

void MainWindow::LoadTableSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("MainWindow");
    m_settings->OpenGroup();
    ui->tbl_Requests->horizontalHeader()->restoreState(m_settings->getParam("MainTableGeometry").toByteArray());
    m_settings->CloseGroup();
    delete m_settings;
}

void MainWindow::getColumnsEnabled()
{  
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("MainWindow");
    m_settings->OpenGroup();
    bool isConfig = m_settings->getParam("isConfig").toBool();
    ui->tbl_Requests->horizontalHeader()->restoreState(m_settings->getParam("MainTableGeometry").toByteArray());

    for (int i = 0; i < m_AccessLevel->getColumnCount(); i++) {
        if (m_AccessLevel->getColumnsList()->value(i)) {
            if (isConfig) {
                m_ColumnsActions.value(i)->setChecked(!ui->tbl_Requests->horizontalHeader()->isSectionHidden(i));
            }else {
                ui->tbl_Requests->setColumnHidden(i, false);
            }
        }else {
            ui->tbl_Requests->setColumnHidden(i, true);
            if (m_ColumnsActions.value(i) != nullptr) m_ColumnsActions.value(i)->setVisible(false);
        }
    }
    m_settings->setParam("isConfig", true);
    m_settings->setParam("MainTableGeometry", ui->tbl_Requests->horizontalHeader()->saveState());
    m_settings->CloseGroup();
    delete m_settings;
}

void MainWindow::getActionsEnabled()
{
    for (int i = 0; i < m_AccessLevel->getActionsCount(); i++) {
        m_ActionsList.value(i)->setVisible(m_AccessLevel->getActionAccessList()->value(i));
    }
}

void MainWindow::usr_setAccsessFilter()
{
    mainTableModel->SetFilter(m_DBFilter->getFilter());
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
    m_DBFilter = new db::clDBFilter(m_AccessLevel->getStatus(), this);
    QObject::connect(m_DBFilter, SIGNAL(filter_changed()), this, SLOT(usr_setAccsessFilter()));
    usr_setAccsessFilter();
    ui->tbl_Requests->selectRow(0);
    ui->tbl_Requests->setVisible(true);
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
    int statusID {mainTableModel->data(mainTableModel->index(current.row(), 16), Qt::DisplayRole).toInt()};

    if (m_AccessLevel->getActionAccessList()->value(1)) {
        if (ui->tbl_Requests->currentIndex().row() > -1) {
            ui->act_ReqEdit->setEnabled(true);
        }else {
            ui->act_ReqEdit->setEnabled(false);
        }
    }

    if (m_AccessLevel->getActionAccessList()->value(2)) {
        if (statusID == 1) {
            ui->act_AcceptRequest->setEnabled(true);
        }else {
            ui->act_AcceptRequest->setEnabled(false);
        }
    }

    if (m_AccessLevel->getActionAccessList()->value(3)) {
        if (ui->tbl_Requests->currentIndex().row() > -1) {
            ui->act_ChangeEngineer->setEnabled(true);
        }else {
            ui->act_ChangeEngineer->setEnabled(false);
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

    if (m_AccessLevel->getActionAccessList()->value(6)) {
        if (ui->tbl_Requests->currentIndex().row() > -1) {
            ui->act_History->setEnabled(true);
        }else {
            ui->act_History->setEnabled(false);
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
    ui->tbl_Requests->selectionModel()->setCurrentIndex(cID, QItemSelectionModel::Rows);
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
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    int stateID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 16), Qt::DisplayRole).toInt()};
    ui_ChangeStatus = new uiChangeStatus(std::make_pair(sendID, dbUserID), stateID, this);
    ui_ChangeStatus->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_ChangeStatus, SIGNAL(usr_ReqStatus_update()), this, SLOT(on_act_Refresh_triggered()));
    ui_ChangeStatus->open();
}

void MainWindow::on_act_ReqClose_triggered()
{
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 0), Qt::DisplayRole).toInt()};
    ui_ChangeStatus = new uiChangeStatus(std::make_pair(sendID, dbUserID), 6, this);
    ui_ChangeStatus->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui_ChangeStatus, SIGNAL(usr_ReqStatus_update()), this, SLOT(on_act_Refresh_triggered()));
    ui_ChangeStatus->open();
}

void MainWindow::on_act_SetCost_triggered()
{
    int sendID {mainTableModel->data(mainTableModel->index(ui->tbl_Requests->currentIndex().row(), 17), Qt::DisplayRole).toInt()};
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
    QMessageBox::about(this, "Информация о программе", "Программа для работы с заявками. \nСалон Охранных Систем \nВерсия " + QApplication::applicationVersion());
}

void MainWindow::on_act_ColNumber_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(0, !checked);
}

void MainWindow::on_act_ColContext_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(1, !checked);
}

void MainWindow::on_act_ColComment_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(2, !checked);
}

void MainWindow::on_act_ColType_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(3, !checked);
}

void MainWindow::on_act_ColObject_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(4, !checked);
}

void MainWindow::on_act_ColAddress_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(5, !checked);
}

void MainWindow::on_act_ColPerson_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(6, !checked);
}

void MainWindow::on_act_ColTel_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(7, !checked);
}

void MainWindow::on_act_ColEmail_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(8, !checked);
}

void MainWindow::on_act_ColData_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(9, !checked);
}

void MainWindow::on_act_ColStatus_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(10, !checked);
}

void MainWindow::on_act_ColUser_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(11, !checked);
}

void MainWindow::on_act_ColEng_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(12, !checked);
}

void MainWindow::on_act_ColMCost_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(13, !checked);
}

void MainWindow::on_act_ColWCost_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(14, !checked);
}

void MainWindow::on_act_ColSum_triggered(bool checked)
{
    ui->tbl_Requests->setColumnHidden(15, !checked);
}

void MainWindow::on_act_ColAll_triggered()
{
    for (int i = 0; i < m_ColumnsActions.size(); i++) {
        if (m_ColumnsActions.value(i)->isVisible()) {
            m_ColumnsActions.value(i)->triggered(true);
            m_ColumnsActions.value(i)->setChecked(true);
        }
    }
}

void MainWindow::on_act_Filter_triggered(bool checked)
{
    ui->filter_Widget->setVisible(checked);
}
