#pragma once

#include <QObject>
#include <QVariant>
#include <QString>
#include <QSettings>

namespace settings {
class StoreSettings {
public:
    StoreSettings();
    ~StoreSettings();

    QVariant getParam(const QString paramName) const;
    void setParam(const QString paramName, const QVariant paramValue);
    QString getCryptedParam(const QString paramName);
    void setCryptedParam(const QString paramName, const QString paramValue);
    QString getGroupName() const;
    void setGroupName(const QString grName);
    void CloseGroup() const;
    void OpenGroup() const;

private:
    QString groupName;
    QSettings* m_settings;
};
}
