#ifndef EDITOTDERSTATUS_H
#define EDITOTDERSTATUS_H

#include <QDialog>

namespace Ui {
class EditOtderStatus;
}

class EditOtderStatus : public QDialog
{
    Q_OBJECT

public:
    explicit EditOtderStatus(QWidget *parent = nullptr);
    ~EditOtderStatus();

private slots:
    void on_EditOtderStatus_accepted();
    void on_EditOtderStatus_rejected();

private:
    Ui::EditOtderStatus *ui;
    void SaveDialogSettings();
    void LoadDialogSettings();
};

#endif // EDITOTDERSTATUS_H
