#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "uconnect_db.h"
#include "ueditrequest.h"
#include "clDBMainQueryModel.h"
#include "uiaddnewwizard.h"
#include "uichooseengineer.h"

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
    QLabel *sts_username;
    QString curUser;
    UEditRequest *ui_editRequest;
    uiAddNewWizard *ui_AddNewWizard;
    uiChooseEngineer *ui_ChooseEngineer;
    db::clDBMainQueryModel* mainTableModel;
    enum class ConnectionDlgMode { StartMode, RunMode};
    int dbUserID;
    QModelIndex editID;
    QString dbUserName;
    void RunConnectionDialog(ConnectionDlgMode mode);
    void ConfigStatusBar();

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
};
#endif // MAINWINDOW_H
