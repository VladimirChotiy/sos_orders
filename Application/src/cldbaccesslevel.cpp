#include "cldbaccesslevel.h"
#include "DBProcessor.h"
#include <QSqlQuery>

namespace db {

clDBAccessLevel::clDBAccessLevel(int id, QObject *parent) :
    QObject(parent),
    userID(id)
{
    DBProcessor* m_DBProcessor {new DBProcessor()};
    QSqlQuery accessQuery {m_DBProcessor->prepareQuery(DBTypes::QueryType::Accsess, userID)};

    accessQuery.first();

    statusMin = accessQuery.value("status_min").toInt();
    statusMax = accessQuery.value("status_max").toInt();

    accessName = accessQuery.value(1).toString();

    actions.append(accessQuery.value("act_ReqRegister").toBool());
    actions.append(accessQuery.value("act_ReqEdit").toBool());
    actions.append(accessQuery.value("act_ReqAccept").toBool());
    actions.append(accessQuery.value("act_ChangeEng").toBool());
    actions.append(accessQuery.value("act_ChangeStatus").toBool());
    actions.append(accessQuery.value("act_ReqCost").toBool());
    actions.append(accessQuery.value("act_History").toBool());
    actions.append(accessQuery.value("act_ReqClose").toBool());

    columns.append(true);
    columns.append(accessQuery.value("col_Context").toBool());
    columns.append(accessQuery.value("col_Comment").toBool());
    columns.append(accessQuery.value("col_Type").toBool());
    columns.append(accessQuery.value("col_Object").toBool());
    columns.append(accessQuery.value("col_Address").toBool());
    columns.append(accessQuery.value("col_Person").toBool());
    columns.append(accessQuery.value("col_Tel").toBool());
    columns.append(accessQuery.value("col_Email").toBool());
    columns.append(accessQuery.value("col_Date").toBool());
    columns.append(accessQuery.value("col_Status").toBool());
    columns.append(accessQuery.value("col_User").toBool());
    columns.append(accessQuery.value("col_Engineer").toBool());
    columns.append(accessQuery.value("col_mCost").toBool());
    columns.append(accessQuery.value("col_wCost").toBool());
    columns.append(accessQuery.value("col_Sum").toBool());
    columns.append(false);
    columns.append(false);

    delete m_DBProcessor;
}

clDBAccessLevel::~clDBAccessLevel()
{

}

QList<bool> clDBAccessLevel::getActions() const
{
    return actions;
}

QList<bool> clDBAccessLevel::getColumns() const
{
    return columns;
}

std::pair<int, int> clDBAccessLevel::getStatus() const
{
    return std::make_pair(statusMin, statusMax);
}

int clDBAccessLevel::getColumnCount() const
{
    return columns.size();
}

QString clDBAccessLevel::getAccessName() const
{
    return accessName;
}

QList<bool> *clDBAccessLevel::getColumnsList()
{
    return &columns;
}

QList<bool> *clDBAccessLevel::getActionAccessList()
{
    return &actions;
}

int clDBAccessLevel::getActionsCount() const
{
    return actions.size();
}

}
