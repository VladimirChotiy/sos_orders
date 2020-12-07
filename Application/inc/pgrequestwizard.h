#ifndef PGREQUESTWIZARD_H
#define PGREQUESTWIZARD_H

#include <QWizardPage>
#include <QSqlQueryModel>

namespace Ui {
class pgRequestWizard;
}

class pgRequestWizard : public QWizardPage
{
    Q_OBJECT

public:
    explicit pgRequestWizard(QWidget *parent = nullptr);
    ~pgRequestWizard();
    bool AddRequest();


public slots:
    void setUserID(int value);
    void setObjectID(int value);

private slots:
    void on_cb_Type_currentIndexChanged(int index);

private:
    Ui::pgRequestWizard *ui;
    QSqlQueryModel *m_TypeModel;

    void StartInit();
    int userID {-1};
    int objectID {-1};
    int typeID {-1};
};

#endif // PGREQUESTWIZARD_H
