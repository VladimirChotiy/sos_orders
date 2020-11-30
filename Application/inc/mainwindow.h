#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "uconnect_db.h"
#include "ueditrequest.h"
#include "clDBMainQueryModel.h"
#include "cldbaccesslevel.h"
#include "uiaddnewwizard.h"
#include "uichooseengineer.h"
#include "uichangestatus.h"
#include "uisetcost.h"
#include "uiprotocolview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    UConnect_db *ui_connect;
    QLabel *sts_connection;
    QLabel *sts_accsess;
    QLabel *sts_username;
    UEditRequest *ui_editRequest;
    uiAddNewWizard *ui_AddNewWizard;
    uiChooseEngineer *ui_ChooseEngineer;
    uiChangeStatus *ui_ChangeStatus;
    uiSetCost *ui_SetCost;
    uiProtocolView *ui_ProtocolView;
    db::clDBMainQueryModel* mainTableModel;
    db::clDBAccessLevel * m_AccessLevel;
    enum class ConnectionDlgMode { StartMode, RunMode};
    int dbUserID;
    QString dbUserName;
    QList<QAction*> m_ActionsList;
    QList<QAction*> m_ColumnsActions;
    void RunConnectionDialog(ConnectionDlgMode mode);
    void ConfigStatusBar();
    void SaveDialogSettings();
    void LoadDialogSettings();
    void SaveTableSettings();
    void LoadTableSettings();
    void getColumnsEnabled();
    void getActionsEnabled();

public slots:
    void ConnectToDB();
    void NoChangesConnectionDlg();

private slots:
    void upd_statusBar_dbConnection(bool status);
    void usr_ActionsActivity_check(const QModelIndex &current, const QModelIndex &previous);
    void on_act_DBConnection_triggered();
    void on_act_Exit_triggered();
    void on_act_Register_triggered();
    void on_act_Refresh_triggered();
    void on_act_ReqEdit_triggered();
    void on_act_AcceptRequest_triggered();
    void on_act_ChangeEngineer_triggered();
    void on_act_ChangeStatus_triggered();
    void on_act_ReqClose_triggered();
    void on_act_SetCost_triggered();
    void on_act_History_triggered();
    void on_act_AboutQT_triggered();
    void on_act_About_triggered();

    void on_act_ColNumber_triggered(bool checked);

    void on_act_ColContext_triggered(bool checked);

    void on_act_ColComment_triggered(bool checked);

    void on_act_ColType_triggered(bool checked);

    void on_act_ColObject_triggered(bool checked);

    void on_act_ColAddress_triggered(bool checked);

    void on_act_ColPerson_triggered(bool checked);

    void on_act_ColTel_triggered(bool checked);

    void on_act_ColEmail_triggered(bool checked);

    void on_act_ColStatus_triggered(bool checked);

    void on_act_ColUser_triggered(bool checked);

    void on_act_ColEng_triggered(bool checked);

    void on_act_ColMCost_triggered(bool checked);

    void on_act_ColWCost_triggered(bool checked);

    void on_act_ColSum_triggered(bool checked);

    void on_act_ColData_triggered(bool checked);

    void on_act_ColAll_triggered();

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
