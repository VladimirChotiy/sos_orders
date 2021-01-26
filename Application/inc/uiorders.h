#ifndef UIORDERS_H
#define UIORDERS_H

#include <QDialog>
#include <QMap>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
class QDateTime;
class QHBoxLayout;
class QToolButton;
//class QSqlQuery;
QT_END_NAMESPACE

namespace Ui {
class uiOrders;
}

class uiOrders : public QDialog
{
    Q_OBJECT

public:
    explicit uiOrders(int id, QWidget *parent = nullptr);
    ~uiOrders();

    enum class DocType {
        dtError,
        dtDefault,
        dtMSWord,
        dtMSExcel,
        dtPdf
    };

private slots:
    void on_pb_AddOrder_clicked();

    void on_uiOrders_accepted();

private:
    Ui::uiOrders *ui;
    void RefreshFilesList();
    QHBoxLayout* CreateLayout(int id, const QString fileName, QDateTime date, int docType = 0);
    std::pair<QString, DocType> AttachFile();
    bool UpdateDBInfo(const QString fileName, DocType docType = DocType::dtDefault);
    void GetOrdersQuery();
    void deleteButton_clicked(QToolButton *tButton);
    void SaveDialogSettings();
    void LoadDialogSettings();
    int reqID;
    QString homeDir;
    QMap<QHBoxLayout*, int> layoutsMap;
    QMap<QToolButton*, int> buttonsMap;
    QSqlQuery ordersQuery;
};

#endif // UIORDERS_H
