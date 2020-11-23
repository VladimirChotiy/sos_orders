#ifndef UIADDNEWWIZARD_H
#define UIADDNEWWIZARD_H

#include <QWizard>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include "pgpersonwizard.h"

namespace Ui {
class uiAddNewWizard;
}

class uiAddNewWizard : public QWizard
{
    Q_OBJECT

public:
    explicit uiAddNewWizard(int userID, QWidget *parent = nullptr);
    ~uiAddNewWizard();


private:
    Ui::uiAddNewWizard *ui;
    pgPersonWizard *personPage;

    void SaveDialogSettings();
    void LoadDialogSettings();
    int dbUserID;
};

#endif // UIADDNEWWIZARD_H
