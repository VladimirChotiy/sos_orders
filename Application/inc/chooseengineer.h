#ifndef CHOOSEENGINEER_H
#define CHOOSEENGINEER_H

#include <QDialog>

namespace Ui {
class ChooseEngineer;
}

class ChooseEngineer : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseEngineer(QWidget *parent = nullptr);
    ~ChooseEngineer();

private slots:
    void on_ChooseEngineer_rejected();

    void on_ChooseEngineer_accepted();

private:
    Ui::ChooseEngineer *ui;
    void LoadDialogSettings();
    void SaveDialogSettings();
};

#endif // CHOOSEENGINEER_H
