#include "cldbfilter.h"
#include <QDebug>
namespace db {

clDBFilter::clDBFilter(std::pair<int, int> range, QObject *parent) :
    QObject(parent)
{
    int minStatus;
    int maxStatus;
    std::tie(minStatus, maxStatus) = range;
    accessFilter = QString("tbl_status.id > %1 AND tbl_status.id < %2 ").arg(minStatus).arg(maxStatus);
    emit filter_changed();
}

clDBFilter::~clDBFilter()
{

}

QString clDBFilter::getFilter()
{
    if (closesFilter) {
        return "WHERE ((" + accessFilter + ") OR (tbl_status.id >= 7 AND tbl_status.id <= 9)) " + statusFilter + engineerFilter + dateFilter;;
    }else {
        return "WHERE " + accessFilter + statusFilter + engineerFilter + dateFilter;
    }
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

void clDBFilter::setDateFilter(QDateTime begin, QDateTime end)
{
    dateFilter ="AND tbl_changes.date >= (SELECT '" + begin.toString("yyyy-MM-dd hh:mm:ss") + "') AND tbl_changes.date <= (SELECT '" + end.toString("yyyy-MM-dd hh:mm:ss") + "') ";
    emit filter_changed();
}

void clDBFilter::setClosesFilter()
{
    closesFilter = true;
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

void clDBFilter::clearClosesFilter()
{
    closesFilter = false;
    emit filter_changed();
}

}
