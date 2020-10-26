#ifndef DBCONNECTIONMANAGER_H
#define DBCONNECTIONMANAGER_H
#include <memory>
#include <QObject>
#include "dbtypes.h"

namespace db
{
class ConnectionManager
{
public:
    Q_DISABLE_COPY(ConnectionManager)
    ~ConnectionManager();
    static ConnectionManager& instance();
    DBTypes::DBState state() const;
    bool isValid() const;

private:
    ConnectionManager();
    class DBManagerPrivate;
    std::unique_ptr<DBManagerPrivate> m_d;
};
}
#endif // DBMDBCONNECTIONANAGER_H
