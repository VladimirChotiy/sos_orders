#ifndef CLDBFILTER_H
#define CLDBFILTER_H

#include <QObject>
#include <QString>
#include <QDateTime>

namespace db {

class clDBFilter : public QObject
{
    Q_OBJECT
public:
    explicit clDBFilter(std::pair<int, int> range, QObject *parent = nullptr);
    ~clDBFilter();
    QString getFilter();
    void setStatusFilter(int id);
    void setEngineerFilter(int id);
    void setDateFilter(const QDateTime &begin, const QDateTime &end);
    void setClosesFilter();
    void clearStatusFilter();
    void clearEngineerFilter();
    void clearDateFilter();
    void clearClosesFilter();

private:
    QString accessFilter {""};
    QString statusFilter {""};
    QString engineerFilter {""};
    QString dateFilter {""};
    bool closesFilter {false};

signals:
    void filter_changed();
};
}

#endif // CLDBFILTER_H
