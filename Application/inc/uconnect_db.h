#pragma once

#include <QDialog>
#include <QTimer>

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
    //settings::StoreSettings* db_settings;
    void GetConnectionSettings();

    // QDialog interface
public slots:
    void checkAutoconnect();

signals:
    emit void timerShot();

};
