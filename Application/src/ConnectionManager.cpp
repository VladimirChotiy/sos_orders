#include "ConnectionManager.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "settings.h"

using namespace DBTypes;
namespace db
{
namespace
{
    class DBCloser {
    public:
        void operator() (QSqlDatabase* db) {
            db->close();
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        }
    };
}

class ConnectionManager::DBManagerPrivate {
public:
    std::unique_ptr<QSqlDatabase, DBCloser> m_database;
    std::string m_dbPath;
    DBState m_state {DBState::OK};
    bool m_isValid {true};
    bool setUp();
    void setIsValid(bool isValid);
};

ConnectionManager& ConnectionManager::instance()
{
    static ConnectionManager instance {};
    return instance;
}

DBState ConnectionManager::state() const
{
    return m_d->m_state;
}

bool ConnectionManager::DBManagerPrivate::setUp()
{
    const QString driver {"QMYSQL"};

    if (!QSqlDatabase::isDriverAvailable(driver))
    {
        m_state = DBState::ERROR_NO_DRIVER;
        qWarning() << "Driver " << driver << " is not available.";
        return false;
    }

    settings::StoreSettings m_settings;
    m_settings.setGroupName("Connection");
    m_settings.OpenGroup();
    QString db_Hostname {m_settings.getParam("Hostname").toString()};
    QString db_User {m_settings.getParam("User").toString()};
    QString db_Password {m_settings.getCryptedParam("Password")};
    bool db_Auto {m_settings.getParam("Autoconnect").toBool()};
    if (!db_Auto) {
        m_settings.setCryptedParam("Password", "");
    }
    m_settings.CloseGroup();

    auto* db = new QSqlDatabase {QSqlDatabase::addDatabase(driver)};
    m_database.reset(db);
    m_database->setHostName(db_Hostname);
    m_database->setUserName(db_User);
    m_database->setPassword(db_Password);
#ifdef QT_DEBUG
    m_database->setDatabaseName("udb_orders_debug");
#else
    m_database->setDatabaseName("udb_orders");
#endif

    if (!m_database->open())
    {
        m_state = DBState::ERROR_OPENING;
        qCritical() << "Error in opening DB " << m_database->databaseName()
                   << " reason: " <<  m_database->lastError().text();
        return false;
    }

    return true;
}

void ConnectionManager::DBManagerPrivate::setIsValid(bool isValid)
{
    m_isValid = isValid;
}

bool ConnectionManager::isValid() const
{
    return m_d->m_isValid && m_d->m_database->isValid();
}

ConnectionManager::ConnectionManager()
    : m_d {new DBManagerPrivate {}}
{
    const bool setupResult {m_d->setUp()};
    m_d->setIsValid(setupResult);
}

ConnectionManager::~ConnectionManager()
{
}
}
