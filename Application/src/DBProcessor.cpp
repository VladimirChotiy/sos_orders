#include "DBProcessor.h"
#include <QSqlError>
#include <QDebug>

using namespace DBTypes;

namespace db
{
DBProcessor::DBProcessor()
    : m_connectionManager {ConnectionManager::instance()}
{
}

DBProcessor::~DBProcessor()
{
}

QString DBProcessor::findUser(const QString curUser)
{
    const QString queryText {"SELECT disp_name FROM tbl_users WHERE name='" + curUser + "'"};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};

    std::tie(result, resultQuery) = Execute(queryText);
    if (result != DBTypes::DBResult::OK) {
        return "Unknow";
    }
    resultQuery.next();
    return resultQuery.value(0).toString();
}

std::pair<DBResult, QSqlQuery> DBProcessor::Execute(const QString& queryText, const QVariantList& args)
{
    if (!m_connectionManager.isValid())
    {
        qCritical() << "Database is not valid, skip";
        return std::make_pair(DBResult::FAIL, QSqlQuery {});
    }
    QSqlQuery query {queryText};

    for(int i = 0; i < args.size(); ++i)
    {
        query.bindValue(i, args[i]);
    }

    DBResult result {DBResult::OK};

    if(!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text()<< query.lastQuery();
        result = DBResult::FAIL;
    }

    return std::make_pair(result, query);
}
}
