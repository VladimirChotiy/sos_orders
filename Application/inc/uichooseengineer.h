#ifndef UICHOOSEENGINEER_H
#define UICHOOSEENGINEER_H

#include <QDialog>

namespace Ui {
class uiChooseEngineer;
}

class uiChooseEngineer : public QDialog
{
    Q_OBJECT

public:
    explicit uiChooseEngineer(int id, QWidget *parent = nullptr);
    ~uiChooseEngineer();

private:
    Ui::uiChooseEngineer *ui;

    int reqID;
    void StartInit();
};

#endif // UICHOOSEENGINEER_H
