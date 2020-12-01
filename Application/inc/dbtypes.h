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
    Person,
    Object,
    Request,
    Status,
    Cost
};

enum class DBUpdateType {
    Request,
    Status,
    Engineer,
    Cost
};

enum class QueryType {
    SystemType,
    Person,
    Object,
    RequestEdit,
    RequestMain,
    Engineer,
    Status,
    Cost,
    Changes,
    Accsess
};

}
#endif // DBTYPES_H
