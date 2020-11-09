#ifndef CLDBEDITREQUEST_H
#define CLDBEDITREQUEST_H

#include <QStringList>
#include "clDBMainQueryModel.h"

namespace db {

class clDBEditRequest : public QObject
{
    Q_OBJECT
public:
    clDBEditRequest(QObject *parent = nullptr);
    clDBEditRequest(int editID, QObject *parent = nullptr);
    ~clDBEditRequest();
    clDBMainQueryModel *GetFullRequestModel() const;
    QStringList GetPartnersList() const;

private:
    clDBMainQueryModel* fullRequestModel;
    bool runMode {false};
};

}

#endif // CLDBEDITREQUEST_H
