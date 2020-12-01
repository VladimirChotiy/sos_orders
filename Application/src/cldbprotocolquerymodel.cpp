#include "cldbprotocolquerymodel.h"
#include "DBProcessor.h"
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QBrush>

namespace db {

clDBProtocolQueryModel::clDBProtocolQueryModel(int index, QObject *parent) :
    QSqlQueryModel(parent),
    reqID(index)
{
    DBProcessor *m_DBProcessor {new DBProcessor()};
    this->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Changes, reqID));
    if (this->lastError().isValid()) {
        qDebug() << this->lastError().text();
        this->clear();
    }
    delete m_DBProcessor;
}

clDBProtocolQueryModel::~clDBProtocolQueryModel()
{

}

void clDBProtocolQueryModel::RefreshQuery()
{
    this->setQuery(this->query().lastQuery());
}

QVariant clDBProtocolQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: return QSqlQueryModel::data(index, role);
    //case Qt::BackgroundRole: return QBrush(Qt::red);
    case Qt::TextAlignmentRole: return GetDataTextAlign(index.column());
    case Qt::FontRole: return QFont("MS Shell Dlg 2", 9);
    default: return QSqlQueryModel::data(index, role);
    }
}

QVariant clDBProtocolQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal){
        return QSqlQueryModel::headerData(section, orientation, role);
    }
    switch (role) {
    case Qt::DisplayRole: return GetHeaderNames(section);
    case Qt::TextAlignmentRole: return QVariant(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    case Qt::FontRole: return QFont("MS Shell Dlg 2", 10, QFont::Bold);
    default: return QSqlQueryModel::headerData(section, orientation, role);
    }
}

QVariant clDBProtocolQueryModel::GetHeaderNames(int column) const
{
    switch (column) {
    case 0:{
            return "Дата";
        }
    case 1:{
            return "Статус";
        }
    case 2:{
            return "Вносивший изменение";
        }
    case 3:{
            return "Комментарий";
        }
    default:{
            return QVariant();
        }
    }
}

QVariant clDBProtocolQueryModel::GetDataTextAlign(int column) const
{
    if ((column == 1) || (column == 2)) {
        return QVariant(Qt::AlignCenter);
    }else{
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }
}

}
