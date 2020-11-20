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

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QSqlQuery resultQuery {};
    const QString selectAllText {"SELECT tbl_requests.id AS reqid, tbl_requests.context, tbl_requests.comment, tbl_types.name AS typename, tbl_objects.name AS objname, tbl_objects.address, tbl_person.name AS persname, tbl_person.telephone, tbl_person.email, tbl_changes.date, tbl_status.name AS statusname, tbl_users.disp_name FROM tbl_requests " \
                "LEFT JOIN tbl_types ON tbl_requests.type_id = tbl_types.id " \
                "LEFT JOIN tbl_objects ON tbl_requests.obj_id = tbl_objects.id " \
                "LEFT JOIN tbl_person ON tbl_objects.parent_id = tbl_person.id " \
                "LEFT JOIN tbl_changes ON tbl_requests.change_id = tbl_changes.id " \
                "LEFT JOIN tbl_status ON tbl_changes.status_id = tbl_status.id " \
                "LEFT JOIN tbl_users ON tbl_changes.user_id = tbl_users.id "};
    QVariant GetHeaderNames(int column) const;
    QVariant GetDataTextAlign(int column) const;
};
}

#endif // CLDBMAINQUERYMODEL_H
