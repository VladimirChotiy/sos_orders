#include "clDBReqInserter.h"
#include "DBProcessor.h"

db::clDBReqInserter::clDBReqInserter(QObject *parent) :
    QObject(parent)
{

}

db::clDBReqInserter::~clDBReqInserter()
{

}

int db::clDBReqInserter::AddPersonInfo(int id, const QString name, const QString telephone, const QString email)
{
    DBProcessor *m_Prosessor {new DBProcessor()};
    QVariantList sendArgs {name, telephone, email};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};

    if (id > -1) {
        const QString textCheckUserID {QString("SELECT id FROM tbl_person WHERE id=%1").arg(id)};
        std::tie(result, resultQuery) = m_Prosessor->Execute(textCheckUserID);
        if (isError(result)) {
            qDebug() << "Error checking user ID";
            delete m_Prosessor;
            return -1;
        }

        if (resultQuery.next()) {
            delete m_Prosessor;
            return id;
        } else {
            qDebug() << "Person ID not found";
            delete m_Prosessor;
            return -1;
        }
    }

    std::tie(result, resultQuery) = m_Prosessor->Execute(textInsertPerson, sendArgs);
    if (isError(result)) {
        qDebug() << "Error inserting new Person";
        delete m_Prosessor;
        return -1;
    }
    std::tie(result, resultQuery) = m_Prosessor->Execute(textGetLastID);
    if (isError(result)) {
        qDebug() << "Error gettin last ID";
        delete m_Prosessor;
        return -1;
    }
    resultQuery.next();

    delete m_Prosessor;
    return resultQuery.value(0).toInt();
}

int db::clDBReqInserter::AddObjectInfo(int id, const QString name, const QString address, int parentID)
{
    DBProcessor *m_Prosessor {new DBProcessor()};
    QVariantList sendArgs {name, address, parentID};
    DBTypes::DBResult result {DBTypes::DBResult::OK};
    QSqlQuery resultQuery {};

    if (id > -1) {
        const QString textCheckObjectID {QString("SELECT id FROM tbl_objects WHERE id=%1").arg(id)};
        std::tie(result, resultQuery) = m_Prosessor->Execute(textCheckObjectID);
        if (isError(result)) {
            qDebug() << "Error checking Object ID";
            delete m_Prosessor;
            return -1;
        }

        if (resultQuery.next()) {
            delete m_Prosessor;
            return id;
        } else {
            qDebug() << "Object ID not found";
            delete m_Prosessor;
            return -1;
        }
    }

    std::tie(result, resultQuery) = m_Prosessor->Execute(textInsertObject, sendArgs);
    if (isError(result)) {
        qDebug() << "Error inserting new Object";
        delete m_Prosessor;
        return -1;
    }
    std::tie(result, resultQuery) = m_Prosessor->Execute(textGetLastID);
    if (isError(result)) {
        qDebug() << "Error gettin last ID";
        delete m_Prosessor;
        return -1;
    }
    resultQuery.next();

    delete m_Prosessor;
    return resultQuery.value(0).toInt();
}

bool db::clDBReqInserter::isError(DBTypes::DBResult result)
{
    if (result == DBTypes::DBResult::OK){
        return false;
    }else {
        return true;
    }
}
