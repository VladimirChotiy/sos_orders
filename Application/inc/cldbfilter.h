#ifndef CLDBFILTER_H
#define CLDBFILTER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QDateTime;
class QString;
class QAction;
QT_END_NAMESPACE

namespace db {

class clDBFilter : public QObject
{
    Q_OBJECT
public:
    explicit clDBFilter(QList<QAction*> actionList, QObject *parent = nullptr);
    ~clDBFilter();
    QString getFilter();
    void setStatusFilter(int id);
    void setEngineerFilter(int id);
    void setDateFilter(const QDateTime &begin, const QDateTime &end);
    void clearEngineerFilter();
    void clearDateFilter();

public slots:
    void usr_fStatusFilter_changed(QAction *action);

private:
    QStringList prepareStatus {};
    QString statusFilter {""};
    QString engineerFilter {""};
    QString dateFilter {""};
    QList<QAction*> statusList;

signals:
    void filter_changed();
};
}

#endif // CLDBFILTER_H
