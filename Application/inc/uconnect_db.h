#ifndef UCONNECTDB_H
#define UCONNECTDB_H

#include <QDialog>
#include <QTimer>
#include "settings.h"

namespace Ui {
class UConnect_db;
}

class UConnect_db : public QDialog
{
    Q_OBJECT

public:
    explicit UConnect_db(QWidget *parent = nullptr);
    ~UConnect_db();
    QTimer *tmr_Autoconnect;

private slots:
    void on_UConnect_db_accepted();
    void on_UConnect_db_rejected();

private:
    Ui::UConnect_db *ui;
    void LoadConnectionSettings();
    void SaveConnectionSettings();
    void LoadDialogUiSettings();
    void SaveDialogUiSettings();

    // QDialog interface
public slots:
    void checkAutoconnect();

signals:
    emit void timerShot();

};

#endif
