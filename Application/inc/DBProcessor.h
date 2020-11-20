#pragma once
#include <QMap>
#include <QDebug>
#include <QSqlQuery>
#include "ConnectionManager.h"

namespace db
{
class DBProcessor
{
public:
    DBProcessor();
    Q_DISABLE_COPY(DBProcessor)
    ~DBProcessor();
    std::pair<int, QString> findUser(const QString curUser);
    std::pair<DBTypes::DBResult, QSqlQuery> SelectMainQuery();
    std::pair<DBTypes::DBResult, QSqlQuery> Execute(const QString& queryText, const QVariantList& args = {});
    QSqlQuery prepareQuery(DBTypes::QueryType qType, int index = -1);

private:
    ConnectionManager& m_connectionManager;
};
}
