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
    Status
};

enum class DBUpdateType {
    Request,
    Status,
    Engineer
};

enum class QueryType {
    SystemType,
    Person,
    Object,
    RequestEdit,
    RequestMain,
    Engineer
};

}
#endif // DBTYPES_H
