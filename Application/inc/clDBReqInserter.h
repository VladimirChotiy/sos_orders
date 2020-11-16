#ifndef CLDBREQINSERTER_H
#define CLDBREQINSERTER_H

#include <QObject>
#include "DBProcessor.h"
#include "dbtypes.h"

namespace db {

class clDBReqInserter : public QObject
{
    Q_OBJECT
public:
    explicit clDBReqInserter(QObject *parent);
    ~clDBReqInserter();

    int AddPersonInfo(int id, const QString name, const QString telephone, const QString email);
    int AddObjectInfo(int id, const QString name, const QString address, int parentID);
    bool AddRequestInfo(int type_id, int obj_id, const QString context);

private:
    bool isError(DBTypes::DBResult result);

    const QString textInsertPerson {"INSERT INTO tbl_person (name, telephone, email) VALUES (?, ?, ?)"};
    const QString textInsertObject {"INSERT INTO tbl_objects (name, address, parent_id) VALUES (?, ?, ?)"};
    const QString textGetLastID {"SELECT LAST_INSERT_ID()"};
    //---------------------
    QString obj_name {};
    QString obj_address {};
    int obj_parentID {-1};
    //---------------------
    int request_type {0};
    QString request_context {};
};

}

#endif // CLDBREQINSERTER_H
