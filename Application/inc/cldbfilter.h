#ifndef CLDBFILTER_H
#define CLDBFILTER_H

#include <QObject>
#include <QString>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

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
    void clearStatusFilter();
    void clearEngineerFilter();
    void clearDateFilter();

public slots:
    void usr_fStatusFilter_changed(QAction *action);
    void usr_fStatusPrepared_changed();

private:
    QString accessFilter {""};
    QStringList prepareStatus {};
    QString statusFilter {""};
    QString engineerFilter {""};
    QString dateFilter {""};

signals:
    void filter_changed();
};
}

#endif // CLDBFILTER_H
