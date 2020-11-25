#include "clDBReqInserter.h"
#include "DBProcessor.h"

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
    case DBTypes::DBInsertType::Person: {
        textQuery = "INSERT INTO tbl_person (name, telephone, email) VALUES (?, ?, ?)";
        errorType = "PERSON";
        break;
    }
    case DBTypes::DBInsertType::Object: {
        textQuery = "INSERT INTO tbl_objects (name, address, parent_id) VALUES (?, ?, ?)";
        errorType = "OBJECT";
        break;
    }
    case DBTypes::DBInsertType::Request: {
        textQuery = "INSERT INTO tbl_requests (obj_id, type_id, context) VALUES (?, ?, ?)";
        errorType = "REQUEST";
        break;
    }
    case DBTypes::DBInsertType::Status: {
        textQuery = "INSERT INTO tbl_changes (status_id, parent_id, comment, user_id) VALUES (?, ?, ?, ?)";
        errorType = "CHANGE";
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
        textQuery = QString("UPDATE tbl_requests SET obj_id=?, type_id=?, context=? WHERE id=%1").arg(id);
        errorType = "Request";
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
