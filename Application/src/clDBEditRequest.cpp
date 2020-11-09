#include "clDBEditRequest.h"

namespace db {

clDBEditRequest::clDBEditRequest(QObject *parent)
    : QObject(parent),
      runMode(false)
{

}

clDBEditRequest::clDBEditRequest(int editID, QObject *parent)
    : QObject(parent),
      fullRequestModel(new clDBMainQueryModel(editID, this)),
      runMode(true)
{

}

clDBEditRequest::~clDBEditRequest()
{

}

clDBMainQueryModel *clDBEditRequest::GetFullRequestModel() const
{
    if (runMode) {
        fullRequestModel->RefreshQuery();
        return fullRequestModel;
    }else return {};
}

QStringList clDBEditRequest::GetPartnersList() const
{
    QStringList result {"", "test1", "test2", "test3"};
    return result;
}

}
