#ifndef UIADDNEWWIZARD_H
#define UIADDNEWWIZARD_H

#include <QWizard>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include "pgobjectwizard.h"
#include "pgrequestwizard.h"

namespace Ui {
class uiAddNewWizard;
}

class uiAddNewWizard : public QWizard
{
    Q_OBJECT

public:
    explicit uiAddNewWizard(int userID, QWidget *parent = nullptr);
    ~uiAddNewWizard();


private slots:
    void on_uiAddNewWizard_rejected();
    void on_uiAddNewWizard_accepted();

private:
    Ui::uiAddNewWizard *ui;
    pgObjectWizard *objectPage;
    pgRequestWizard *requestPage;

    void SaveDialogSettings();
    void LoadDialogSettings();
    int dbUserID;

signals:
    void usr_NewRequest_added();
};

#endif // UIADDNEWWIZARD_H
