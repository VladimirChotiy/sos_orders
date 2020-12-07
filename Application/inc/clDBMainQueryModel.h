#ifndef CLDBMAINQUERYMODEL_H
#define CLDBMAINQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>

namespace db
{
class clDBMainQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit clDBMainQueryModel(QObject *parent = nullptr);
    ~clDBMainQueryModel();
    void RefreshQuery();
    void SetFilter(const QString &filter);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    void sort(int column, Qt::SortOrder order);

private:
    QVariant GetHeaderNames(int column) const;
    QVariant GetDataTextAlign(int column) const;
    QString sortString {};
    QString filterString {};
    QStringList columNames {};
};
}

#endif // CLDBMAINQUERYMODEL_H
