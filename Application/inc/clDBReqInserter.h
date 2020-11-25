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
    void UpdateData(int id, QVariantList arg, DBTypes::DBUpdateType type);
    bool UpdateUser(int user_id, int index);

private:
    bool isError(DBTypes::DBResult result);

    const QString textGetLastID {"SELECT LAST_INSERT_ID()"};
};

}

#endif // CLDBREQINSERTER_H
