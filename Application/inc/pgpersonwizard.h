#ifndef PGPERSONWIZARD_H
#define PGPERSONWIZARD_H

#include <QWizardPage>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

namespace Ui {
class pgPersonWizard;
}

class pgPersonWizard : public QWizardPage
{
    Q_OBJECT

public:
    explicit pgPersonWizard(QWidget *parent = nullptr);
    ~pgPersonWizard();
    int getPersonID() const;

private slots:
    void on_pb_AddPerson_clicked();
    void on_pb_SavePerson_clicked();
    void usr_PersonIndex_changed(int index);
    void usr_PersonText_changed(const QString &text);

private:
    Ui::pgPersonWizard *ui;
    QSqlQueryModel *m_PersonModel;
    QDataWidgetMapper *m_PersonMapper;

    enum class PersonMode {Add, Choose};
    PersonMode runPersonMode;

    void SwitchPersonMode(PersonMode mode);
    void StartInit();
    int personID {-1};

signals:
    void usr_PersonId_changed(int id);
};

#endif // PGPERSONWIZARD_H
