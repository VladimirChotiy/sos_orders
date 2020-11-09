#ifndef UREGISTERNEWREQUEST_H
#define UREGISTERNEWREQUEST_H

#include <QDialog>
#include "clDBEditRequest.h"

namespace Ui {
class URegisterNewRequest;
}

class URegisterNewRequest : public QDialog
{
    Q_OBJECT

public:
    explicit URegisterNewRequest(QWidget *parent = nullptr);
    URegisterNewRequest(int editID, QWidget *parent = nullptr);
    ~URegisterNewRequest();

private slots:
    void on_btn_Card_clicked();
    void on_btn_ObjectAddress_clicked();
    void on_URegisterNewRequest_rejected();

    void on_URegisterNewRequest_accepted();

private:
    Ui::URegisterNewRequest *ui;
    db::clDBEditRequest m_fullModel;
    void SaveDialogSettings();
    void LoadDialogSettings();
    void StartInit();
};

#endif // UREGISTERNEWREQUEST_H
