#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "uconnect_db.h"
#include "uregisternewrequest.h"
#include "chooseengineer.h"
#include "editotderstatus.h"
#include "ueditrequest.h"
#include "clDBMainQueryModel.h"

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
    URegisterNewRequest *URegisterRequest;
    QLabel *sts_connection;
    QLabel *sts_username;
    QString curUser;
    ChooseEngineer *ui_chooseEng;
    EditOtderStatus *ui_editOrderStatus;
    UEditRequest *ui_editRequest;
    db::clDBMainQueryModel* mainTableModel;
    enum class ConnectionDlgMode { StartMode, RunMode};
    int dbUserID;
    QModelIndex editID;
    QString dbUserName;
    void RunConnectionDialog(ConnectionDlgMode mode);
    void ConfigStatusBar();
    void RunEditDlg();

public slots:
    void ConnectToDB();
    void NoChangesConnectionDlg();
private slots:
    void upd_statusBar_dbConnection(bool status);
    void on_act_DBConnection_triggered();
    void on_act_Exit_triggered();
    void on_act_Register_triggered();
    void on_act_Accept_triggered();
    void on_act_Survey_triggered();
    void on_act_Refresh_triggered();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
