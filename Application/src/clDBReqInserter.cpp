#include "clDBReqInserter.h"
#include "DBProcessor.h"
#include <QDebug>

namespace db {

clDBReqInserter::clDBReqInserter(QObject *parent) :
    QObject(parent)
{

}

clDBReqInserter::~clDBReqInserter()
{

}

int clDBReqInserter::AddData(QVariantList arg, DBTypes::DBInsertType type)
{
    DBProcessor *m_Prosessor {new DBProcessor()};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};
    QString textQuery {};
    QString errorType;

    switch (type) {
    case DBTypes::DBInsertType::Object: {
        textQuery = "INSERT INTO tbl_objects (name, address, person, telephone, email) VALUES (?, ?, ?, ?, ?)";
        errorType = "OBJECT";
        break;
    }
    case DBTypes::DBInsertType::Request: {
        textQuery = "INSERT INTO tbl_requests (obj_id, type_id, context, cost_id) VALUES (?, ?, ?, ?)";
        errorType = "REQUEST";
        break;
    }
    case DBTypes::DBInsertType::Status: {
        textQuery = "INSERT INTO tbl_changes (status_id, parent_id, comment, user_id) VALUES (?, ?, ?, ?)";
        errorType = "CHANGE";
        break;
    }
    case DBTypes::DBInsertType::Cost: {
        textQuery = "INSERT INTO tbl_cost (m_cost, w_cost) VALUES (?, ?)";
        errorType = "COST";
        break;
    }
    case DBTypes::DBInsertType::Order: {
        textQuery = "INSERT INTO tbl_order (name, type, parent_id) VALUES(?, ?, ?)";
        errorType = "ORDER";
        break;
    }
    default: textQuery = "";
    }

    std::tie(result, resultQuery) = m_Prosessor->Execute(textQuery, arg);
    if (isError(result)) {
        qDebug() << "Error inserting new " << errorType;
        delete m_Prosessor;
        return -1;
    }
    std::tie(result, resultQuery) = m_Prosessor->Execute(textGetLastID);
    if (isError(result)) {
        qDebug() << "Error gettin last ID";
        delete m_Prosessor;
        return -1;
    }
    delete m_Prosessor;
    resultQuery.next();
    return resultQuery.value(0).toInt();
}

void clDBReqInserter::UpdateData(int id, QVariantList arg, DBTypes::DBUpdateType type)
{
    DBProcessor *m_Prosessor {new DBProcessor()};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};
    QString textQuery {};
    QString errorType;

    switch (type) {
    case DBTypes::DBUpdateType::Request: {
        textQuery = QString("UPDATE tbl_requests SET type_id=?, context=? WHERE id=%1").arg(id);
        errorType = "Request";
        break;
    }
    case DBTypes::DBUpdateType::Engineer: {
        textQuery = QString("UPDATE tbl_requests SET resp_id=? WHERE id=%1").arg(id);
        errorType = "Engineer";
        break;
    }   
    case DBTypes::DBUpdateType::Cost: {
        textQuery = QString("UPDATE tbl_cost SET m_cost=?, w_cost=? WHERE id = %1").arg(id);
        errorType = "Cost";
        break;
    }
    case DBTypes::DBUpdateType::Object: {
        textQuery = QString("UPDATE tbl_objects SET name=?, address=?, person=?, telephone=?, email=? WHERE id = %1").arg(id);
        errorType = "Object";
        break;
    }
    case DBTypes::DBUpdateType::Order: {
        textQuery = QString("UPDATE tbl_order SET name=?, type=?, parent_id=? WHERE id = %1").arg(id);
        errorType = "Order";
        break;
    }

    default: textQuery = "";
    }
    std::tie(result, resultQuery) = m_Prosessor->Execute(textQuery, arg);
    if (isError(result)) {
        qDebug() << "Error updating " << errorType;
    }
    delete m_Prosessor;
}

bool clDBReqInserter::UpdateUser(int user_id, int index)
{
    DBProcessor *m_Prosessor {new DBProcessor()};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};
    QString textQuery {QString("UPDATE tbl_changes SET user_id=%1 WHERE parent_id=%2 ORDER BY id DESC LIMIT 1").arg(user_id).arg(index)};

    std::tie(result, resultQuery) = m_Prosessor->Execute(textQuery);
    if (isError(result)) {
        qDebug() << "Error updating status";
        delete m_Prosessor;
        return false;
    }
    delete  m_Prosessor;
    return true;
}

bool clDBReqInserter::isError(DBTypes::DBResult result)
{
    if (result == DBTypes::DBResult::OK){
        return false;
    }else {
        return true;
    }
}

}
