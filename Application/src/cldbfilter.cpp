#include "cldbfilter.h"
#include <QAction>
#include <QDebug>
namespace db {

clDBFilter::clDBFilter(std::pair<int, int> range, QObject *parent) :
    QObject(parent)
{
    int minStatus;
    int maxStatus;
    int listSize;
    std::tie(minStatus, maxStatus) = range;
    //accessFilter = QString("tbl_status.id > %1 AND tbl_status.id < %2 ").arg(minStatus).arg(maxStatus);
    listSize = (maxStatus - minStatus - 1);
    prepareStatus.reserve(listSize);
    for (int i = 0; i < listSize; i++){
        prepareStatus.append("");
    }
    qDebug() << prepareStatus.size();
    accessFilter = "tbl_status.id > 0 AND tbl_status.id < 10 ";
    emit filter_changed();
}

clDBFilter::~clDBFilter()
{

}

QString clDBFilter::getFilter()
{
    return "WHERE " + accessFilter + statusFilter + engineerFilter + dateFilter;
}

void clDBFilter::setStatusFilter(int id)
{
    statusFilter = QString("AND tbl_status.id = %1 ").arg(id);

    emit filter_changed();
}

void clDBFilter::setEngineerFilter(int id)
{
    engineerFilter = QString("AND tbl_requests.resp_id = %1 ").arg(id);
    emit filter_changed();
}

void clDBFilter::setDateFilter(const QDateTime &begin, const QDateTime &end)
{
    dateFilter ="AND tbl_changes.date >= (SELECT '" + begin.toString("yyyy-MM-dd hh:mm:ss") + "') AND tbl_changes.date <= (SELECT '" + end.toString("yyyy-MM-dd hh:mm:ss") + "') ";
    emit filter_changed();
}

void clDBFilter::clearStatusFilter()
{
    statusFilter = "";
    emit filter_changed();
}

void clDBFilter::clearEngineerFilter()
{
    engineerFilter = "";
    emit filter_changed();
}

void clDBFilter::clearDateFilter()
{
    dateFilter = "";
    emit filter_changed();
}

void clDBFilter::usr_fStatusFilter_changed(QAction *action)
{
    if (action->isChecked()) {
        prepareStatus[action->data().toInt()] = QString("OR tbl_status.id = %1 ").arg(action->data().toInt());
    }else {
        prepareStatus.value(action->data().toInt()) = "";
    }
    qDebug() << prepareStatus.value(action->data().toInt());
    usr_fStatusPrepared_changed();
}

void clDBFilter::usr_fStatusPrepared_changed()
{
    QString filter {""};
    for (int i = 0; i < prepareStatus.size(); i++){
        filter += prepareStatus.value(i);
    }
    qDebug() << filter;
}

}
