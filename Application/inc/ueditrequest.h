#ifndef UEDITREQUEST_H
#define UEDITREQUEST_H

#include <QDialog>

namespace Ui {
class ueditrequest;
}

class UEditRequest : public QDialog
{
    Q_OBJECT

public:
    explicit UEditRequest(QWidget *parent = nullptr);
    ~UEditRequest();

private slots:
    void on_ueditrequest_accepted();

    void on_ueditrequest_rejected();

private:
    Ui::ueditrequest *ui;
};

#endif // UEDITREQUEST_H
