#ifndef CLDBACCESSLEVEL_H
#define CLDBACCESSLEVEL_H

#include <QObject>

namespace db {

class clDBAccessLevel : public QObject
{
    Q_OBJECT
public:
    explicit clDBAccessLevel(int id, QObject *parent = nullptr);
    ~clDBAccessLevel();
    QList<bool> getActions() const;
    QList<bool> getColumns() const;
    std::pair<int,int> getStatus() const;
    int getActionsCount() const;
    int getColumnCount() const;
    int getAccessID() const;
    QString getAccessName() const;
    QList<bool> *getActionAccessList();
    QList<bool> *getColumnsList();

private:
    int userID;
    QList<bool> actions;
    QList<bool> columns;
    QString accessName;
    int accessID;

    int statusMin;
    int statusMax;
};

}

#endif // CLDBACCESSLEVEL_H
