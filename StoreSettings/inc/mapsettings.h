#pragma once
#include <map>
#include <vector>
#include <QString>

namespace settings {
enum class groupSettings
{
    Connection
};

enum class tpConnectNames {
    Hostname,
    User,
    Password,
    Autoconnect
};

std::map<groupSettings, QString> mapSetGroupNames {
    {groupSettings::Connection, "Connection"}
};

std::map<tpConnectNames, QString> mapConSetNames {
    {tpConnectNames::Hostname, "Hostname"},
    {tpConnectNames::User, "User"},
    {tpConnectNames::Password, "Password"},
    {tpConnectNames::Autoconnect, "Autoconnect"}
};

}
