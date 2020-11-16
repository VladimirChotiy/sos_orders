#ifndef CLDBREQINSERTER_H
#define CLDBREQINSERTER_H

#include <QObject>
#include "dbtypes.h"

namespace db {

class clDBReqInserter : public QObject
{
    Q_OBJECT
public:
    explicit clDBReqInserter(QObject *parent);
    ~clDBReqInserter();

    int AddData(QVariantList arg, DBTypes::DBInsertType type);
    bool UpdateUser(int user_id, int index);

private:
    bool isError(DBTypes::DBResult result);

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
