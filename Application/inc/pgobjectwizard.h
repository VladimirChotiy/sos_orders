#ifndef PGOBJECTWIZARD_H
#define PGOBJECTWIZARD_H

#include <QWizardPage>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QDataWidgetMapper>

namespace Ui {
class pgObjectWizard;
}

class pgObjectWizard : public QWizardPage
{
    Q_OBJECT

public:
    explicit pgObjectWizard(QWidget *parent = nullptr);
    ~pgObjectWizard();
    int getObjectID() const;
    void RefreshObjectPage();

public slots:
    void setPersonID(int value);

private slots:
    void usr_ObjectIndex_changed(int index);
    void usr_ObjectText_changed(const QString &text);
    void on_pb_AddObject_clicked();
    void on_pb_Switch_clicked();

private:
    Ui::pgObjectWizard *ui;
    QSqlQueryModel *m_ObjectModel;
    QSortFilterProxyModel *m_ObjectProxy;
    QDataWidgetMapper *m_ObjectMapper;

    enum class ObjectMode {Add, Choose};
    ObjectMode runObjectMode;

    void SwitchObjectMode(ObjectMode mode);
    void StartInit();
    int personID {-1};
    int objectID {-1};

signals:
    void usr_ObjectId_changed(int id);
};

#endif // PGOBJECTWIZARD_H
