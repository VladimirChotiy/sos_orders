#include "clDBMainQueryModel.h"
#include "DBProcessor.h"
#include "dbtypes.h"
#include <QSqlError>
#include <QDebug>


clDBMainQueryModel::clDBMainQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    db::DBProcessor* m_dbProcessor {new db::DBProcessor()};
    DBTypes::DBResult result {DBTypes::DBResult::OK};

    std::tie(result, resultQuery) = m_dbProcessor->Execute(queryText);
    delete m_dbProcessor;
    if (result == DBTypes::DBResult::OK) {
        this->setQuery(resultQuery);
        if (this->lastError().isValid()) {
            qDebug() << this->lastError().text();
            this->clear();
        }
    }else {
        qDebug() << "Error execute query from DBProcessor";
    }
}

clDBMainQueryModel::~clDBMainQueryModel()
{

}
