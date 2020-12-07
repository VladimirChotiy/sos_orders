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
    DBResult result {DBResult::OK};
    QSqlQuery resultQuery {};

    std::tie(result, resultQuery) = Execute(queryText);

    int resultID{-1};
    QString resultName {"Unknown"};

    if (result != DBResult::OK) {
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

QSqlQuery DBProcessor::prepareQuery(QueryType qType, int index, const QString &filter)
{
    QSqlQuery resultQuery {};
    DBResult result {DBResult::OK};
    QString textQuery {""};

    switch (qType) {
    case QueryType::SystemType: {
       textQuery = "SELECT * FROM tbl_types ORDER BY id";
       break;
    }
    case QueryType::Object: {
        textQuery = QString("SELECT * FROM tbl_objects ORDER BY name");
        break;
    }
    case QueryType::RequestEdit: {
        textQuery = QString("SELECT tbl_requests.id, tbl_requests.context, tbl_changes.comment, tbl_requests.type_id, tbl_requests.obj_id, tbl_requests.change_id, tbl_status.name, tbl_changes.date " \
            "FROM tbl_requests " \
            "LEFT JOIN tbl_changes ON tbl_requests.change_id = tbl_changes.id " \
            "LEFT JOIN tbl_status ON tbl_changes.status_id = tbl_status.id " \
            "LEFT JOIN tbl_users ON tbl_changes.user_id = tbl_users.id " \
            "WHERE tbl_requests.id = %1").arg(index);
        break;
    }
    case QueryType::RequestMain: {
        textQuery = QString("SELECT tbl_requests.id AS reqid, tbl_requests.context, tbl_changes.comment, tbl_types.name AS typename, tbl_objects.name AS objname, tbl_objects.address, tbl_objects.person AS persname, tbl_objects.telephone, tbl_objects.email, tbl_changes.date, tbl_status.name AS statusname, tbl_users.disp_name, (SELECT tbl_users.disp_name FROM tbl_users WHERE tbl_users.id = tbl_requests.resp_id) AS respuser, tbl_cost.m_cost, tbl_cost.w_cost, tbl_cost.sum, tbl_status.id AS statusid, tbl_requests.cost_id FROM tbl_requests " \
            "LEFT JOIN tbl_types ON tbl_requests.type_id = tbl_types.id " \
            "LEFT JOIN tbl_objects ON tbl_requests.obj_id = tbl_objects.id " \
            "LEFT JOIN tbl_changes ON tbl_requests.change_id = tbl_changes.id " \
            "LEFT JOIN tbl_status ON tbl_changes.status_id = tbl_status.id " \
            "LEFT JOIN tbl_users ON tbl_changes.user_id = tbl_users.id " \
            "LEFT JOIN tbl_cost ON tbl_requests.cost_id = tbl_cost.id ");
        break;
    }
    case QueryType::Engineer: {
        textQuery = QString("SELECT * FROM tbl_users WHERE tbl_users.spec_id = %1").arg(index);
        break;
    }
    case QueryType::Status: {
        textQuery = QString("SELECT * FROM tbl_status WHERE tbl_status.id > %1 AND tbl_status.id < 10 ORDER BY tbl_status.id").arg(index);
        break;
    }
    case QueryType::Cost: {
        textQuery = QString("SELECT * FROM tbl_cost WHERE tbl_cost.id = %1").arg(index);
        break;
    }
    case QueryType::Changes: {
        textQuery = QString("SELECT tbl_changes.date, tbl_status.name, tbl_users.disp_name, tbl_changes.comment FROM tbl_changes " \
                            "LEFT JOIN tbl_users ON tbl_changes.user_id = tbl_users.id " \
                            "LEFT JOIN tbl_status ON tbl_changes.status_id = tbl_status.id " \
                            "WHERE tbl_changes.parent_id = %1 ORDER BY tbl_changes.date").arg(index);
        break;
    }
    case QueryType::Accsess: {
        textQuery = QString("SELECT * FROM tbl_specs WHERE tbl_specs.id = (SELECT tbl_users.spec_id FROM tbl_users WHERE tbl_users.id = %1)").arg(index);
        break;
    }
    default: textQuery = "";
    }
    std::tie(result, resultQuery) = Execute(textQuery + filter);
    if (result == DBResult::FAIL) {
        qDebug() << "Error prepare Query";
    }
    return resultQuery;
}
}
