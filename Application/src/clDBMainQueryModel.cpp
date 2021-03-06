#include "clDBMainQueryModel.h"
#include "DBProcessor.h"
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QBrush>

namespace db
{
clDBMainQueryModel::clDBMainQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    columNames << "reqid" << "tbl_requests.context" << "tbl_changes.comment" << "typename" << "objname" << "tbl_objects.address" << "persname" << "tbl_objects.telephone" << "tbl_objects.email" << "tbl_changes.date" << "statusname" << "tbl_users.disp_name" << "respuser" << "tbl_cost.m_cost" << "tbl_cost.w_cost" << "tbl_cost.sum";
    RefreshQuery();
}

clDBMainQueryModel::~clDBMainQueryModel()
{

}

void clDBMainQueryModel::RefreshQuery()
{
    DBProcessor* m_dbProcessor {new DBProcessor()};
    this->setQuery(m_dbProcessor->prepareQuery(DBTypes::QueryType::RequestMain, 0, filterString, sortString));
    if (this->lastError().isValid()) {
        qDebug() << this->lastError().text();
        this->clear();
    }
    delete m_dbProcessor;
}

void clDBMainQueryModel::SetFilter(const QString &filter)
{
    filterString = filter;
    RefreshQuery();
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
    case 12:{
            return "Ответственное лицо";
        }
    case 13:{
            return "Стоимость материалов, руб.";
        }
    case 14:{
            return "Стоимость работ, руб.";
        }
    case 15:{
            return "Общая сумма, руб.";
        }
    case 16:{
            return "status_id (hide)";
        }
    case 17:{
            return "cost_id (hide)";
        }
    default:{
            return QVariant();
        }
    }
}

QVariant clDBMainQueryModel::GetDataTextAlign(int column) const
{
    if ((column == 0) || (column == 3) || ((column >= 6) && (column <=15))) {
        return QVariant(Qt::AlignCenter);
    }else{
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }
}

QVariant clDBMainQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal){
        return QSqlQueryModel::headerData(section, orientation, role);
    }
    switch (role) {
    case Qt::DisplayRole: return GetHeaderNames(section);
    case Qt::TextAlignmentRole: return QVariant(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    case Qt::FontRole: return QFont("MS Shell Dlg 2", 10, QFont::Bold);
    case Qt::SizeHintRole: return QSize(50, 60);
    case Qt::InitialSortOrderRole: return QVariant(Qt::DescendingOrder);
    default: return QSqlQueryModel::headerData(section, orientation, role);
    }

}

QVariant clDBMainQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: return QSqlQueryModel::data(index, role);
//    case Qt::BackgroundRole: {
//        if (this->data(this->index(index.row(), 0), Qt::DisplayRole).toInt() == 1) {
//            return QBrush(Qt::red);
//        }else {
//            return QSqlQueryModel::data(index, role);
//        }
//    }
    case Qt::TextAlignmentRole: return GetDataTextAlign(index.column());
    case Qt::FontRole: return QFont("MS Shell Dlg 2", 9);
    default: return QSqlQueryModel::data(index, role);
    }
}

void clDBMainQueryModel::sort(int column, Qt::SortOrder order)
{
    //sortString = "ORDER BY " + this->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString() + " ";
    sortString = "ORDER BY " + columNames.value(column) + " ";
    if (order == Qt::DescendingOrder) {
        sortString += "DESC";
    }else {
        sortString += "ASC";
    }
    RefreshQuery();
}
}
