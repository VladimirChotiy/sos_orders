#include "settings.h"
#include "simplecrypt.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

namespace settings {

StoreSettings::StoreSettings()
    : groupName {"default"}
{

    QString fileLocation {QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    QString fullPath  {fileLocation + "/" + "settings.cfg"};
    QDir settingsDirectory {fileLocation};
    if (!settingsDirectory.exists()){
        const bool creationResult {settingsDirectory.mkpath(fileLocation)};
        qWarning() << "Settings directory is not exist, creating result: " << creationResult;
    }

    m_settings = new QSettings(fullPath, QSettings::IniFormat);
}

StoreSettings::~StoreSettings(){
    qDebug() << "Delete settings!";
    delete m_settings;
}

QString StoreSettings::getCryptedParam(const QString paramName)
{
    QString getResult {getParam(paramName).toString()};
    SimpleCrypt crypto {Q_UINT64_C(0x3c2af4a4acb9f253)};
    if (paramName.isEmpty()) {
        return {};
    }
    return crypto.decryptToString(getResult);
}

void StoreSettings::setCryptedParam(const QString paramName, const QString paramValue)
{
    QString setParamValue {paramValue};
    SimpleCrypt crypto {Q_UINT64_C(0x3c2af4a4acb9f253)};
    if (setParamValue.isEmpty()) {
        setParam(paramName, "");
    }else {
        setParam(paramName, crypto.encryptToString(paramValue));
    }
}

QVariant StoreSettings::getParam(const QString paramName) const
{
    QVariant value {m_settings->value(paramName)};
    return value;
}

void StoreSettings::setParam(const QString paramName, const QVariant paramValue)
{
    m_settings->setValue(paramName, paramValue);
}

QString StoreSettings::getGroupName() const
{
    return groupName;
}

void StoreSettings::setGroupName(const QString grName)
{
    groupName = grName;
}

void StoreSettings::CloseGroup() const
{
    m_settings->endGroup();
}

void StoreSettings::OpenGroup() const
{
    m_settings->beginGroup(groupName);
}

}
