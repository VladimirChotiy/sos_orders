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

std::pair<int, QString> DBProcessor::findUser(const QString curUser)
{
    const QString queryText {"SELECT id, disp_name FROM tbl_users WHERE name='" + curUser + "'"};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};

    std::tie(result, resultQuery) = Execute(queryText);

    int resultID{-1};
    QString resultName {"Unknown"};

    if (result != DBTypes::DBResult::OK) {
        return std::make_pair(resultID, resultName);
    }
    if (resultQuery.size() > 0){
        resultQuery.next();
        resultID = resultQuery.value(0).toInt();
        resultName = resultQuery.value(1).toString();
    }
    return std::make_pair(resultID, resultName);
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
