#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QSettings>
#include <map>

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
    std::pair<int, int> GetFormGeometry(const QString formName);
    void SetFormGeometry(const QString formName, int fWidth, int fHeigth);

private:
    QString groupName;
    QSettings* m_settings;
};

}

#endif
