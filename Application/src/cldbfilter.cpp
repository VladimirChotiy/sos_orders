#include "cldbfilter.h"
#include <QString>
#include <QDateTime>
#include <QAction>
#include <QDebug>
namespace db {

clDBFilter::clDBFilter(QList<QAction*> actionList, QObject *parent) :
    QObject(parent),
    statusList(actionList)
{
}

clDBFilter::~clDBFilter()
{

}

QString clDBFilter::getFilter()
{
    QString result {"WHERE " + dateFilter + statusFilter + engineerFilter};
    return result;
}

void clDBFilter::setEngineerFilter(int id)
{
    engineerFilter = QString("AND tbl_requests.resp_id = %1 ").arg(id);
    emit filter_changed();
}

void clDBFilter::setDateFilter(const QDateTime &begin, const QDateTime &end)
{
    dateFilter ="tbl_changes.date >= (SELECT '" + begin.toString("yyyy-MM-dd hh:mm:ss") + "') AND tbl_changes.date <= (SELECT '" + end.toString("yyyy-MM-dd hh:mm:ss") + "') ";
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
    Q_UNUSED(action);
    bool isFirst {true};
    bool isEmpty {true};
    statusFilter.clear();
    statusFilter = "AND (";
    for (QAction *a : statusList){
        if (a->isChecked() && a->isVisible()) {
            if (isFirst) {
                statusFilter += a->data().toString();
                isFirst = false;
            } else {
                statusFilter += "OR " + a->data().toString();
            }
            isEmpty = false;
        }
    }
    if (isEmpty) {
        statusFilter += "tbl_status.id = 0) ";
    } else {
        statusFilter.truncate(statusFilter.lastIndexOf(QChar(' ')));
        statusFilter += ") ";
    }
    emit filter_changed();
}

}
