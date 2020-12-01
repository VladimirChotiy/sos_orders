#ifndef CLDBPROTOCOLQUERYMODEL_H
#define CLDBPROTOCOLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QObject>

namespace db {

class clDBProtocolQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit clDBProtocolQueryModel(int index, QObject *parent = nullptr);
    ~clDBProtocolQueryModel();
    void RefreshQuery();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QVariant GetHeaderNames(int column) const;
    QVariant GetDataTextAlign(int column) const;

    int reqID {-1};
};

}
#endif // CLDBPROTOCOLQUERYMODEL_H
