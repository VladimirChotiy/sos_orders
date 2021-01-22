#ifndef DBTYPES_H
#define DBTYPES_H
#include <QVariantList>

namespace DBTypes
{
enum class DBResult {
    OK,
    FAIL
};

enum class DBState {
    OK,
    ERROR_NO_DRIVER,
    ERROR_OPENING
};

enum class DBInsertType {
    Object,
    Request,
    Status,
    Cost,
    Order
};

enum class DBUpdateType {
    Request,
    Status,
    Engineer,
    Cost,
    Object,
    Order
};

enum class QueryType {
    SystemType,
    Object,
    RequestEdit,
    RequestMain,
    Engineer,
    Status,
    Cost,
    Changes,
    Accsess,
    Dates,
    RepCovert,
    RepTitle,
    Order
};

}
#endif // DBTYPES_H
