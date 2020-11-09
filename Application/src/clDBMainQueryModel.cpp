#include "clDBMainQueryModel.h"
#include "DBProcessor.h"
#include "dbtypes.h"
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QBrush>

namespace db
{
clDBMainQueryModel::clDBMainQueryModel(int reqID, QObject *parent)
    : QSqlQueryModel(parent)
{
    DBProcessor* m_dbProcessor {new DBProcessor()};
    DBTypes::DBResult result {DBTypes::DBResult::OK};

    QString sendQueryText;
    if (reqID > 0) {
        sendQueryText = queryText + " WHERE tbl_requests.id = " + QString::number(reqID);
    }else {
        sendQueryText = queryText;
    }
    std::tie(result, resultQuery) = m_dbProcessor->Execute(sendQueryText);
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

void clDBMainQueryModel::RefreshQuery()
{
    this->setQuery(this->query().lastQuery());
}

QVariant clDBMainQueryModel::GetHeaderNames(int column) const
{
    switch (column) {
    case 0:{
            return "№ п/п";
        }
    case 1:{
            return "Описание заявки";
        }
    case 2:{
            return "Комментарий";
        }
    case 3:{
            return "Тип системы";
        }
    case 4:{
            return "Объект";
        }
    case 5:{
            return "Адрес объекта";
        }
    case 6:{
            return "Заказчик";
        }
    case 7:{
            return "Тел. заказчика";
        }
    case 8:{
            return "Почта заказчика";
        }
    case 9:{
            return "Дата изменения";
        }
    case 10:{
            return "Статус заявки";
        }
    case 11:{
            return "Кем внесены изменения";
        }
    default:{
            return QVariant();
        }
    }
}

QVariant clDBMainQueryModel::GetDataTextAlign(int column) const
{
    if ((column == 0) || (column == 3) || ((column >= 6) && (column <=11))) {
        return QVariant(Qt::AlignCenter);
    }else{
        return QVariant(Qt::AlignLeft | Qt::AlignTop);
    }
}

QVariant clDBMainQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal){
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole: return GetHeaderNames(section);
    //case Qt::TextAlignmentRole: return QVariant(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    case Qt::FontRole: return QFont("MS Shell Dlg 2", 10, QFont::Bold);
    default: return QVariant();
    }

}

QVariant clDBMainQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: return QSqlQueryModel::data(index, role);
    //case Qt::BackgroundRole: return QBrush(Qt::red);
    case Qt::TextAlignmentRole: return GetDataTextAlign(index.column());
    case Qt::FontRole: return QFont("MS Shell Dlg 2", 9);
    default: return QVariant();
    }
}
}