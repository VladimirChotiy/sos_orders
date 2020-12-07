#ifndef UIPROTOCOLVIEW_H
#define UIPROTOCOLVIEW_H

#include <QDialog>
#include "cldbprotocolquerymodel.h"

namespace Ui {
class uiProtocolView;
}

class uiProtocolView : public QDialog
{
    Q_OBJECT

public:
    explicit uiProtocolView(int index, QWidget *parent = nullptr);
    ~uiProtocolView();

private slots:
    void on_uiProtocolView_accepted();

private:
    Ui::uiProtocolView *ui;
    db::clDBProtocolQueryModel *m_ProtocolModel;

    int reqID {-1};
    void StartInit();
    void SaveDialogSettings();
    void LoadDialogSettings();
};

#endif // UIPROTOCOLVIEW_H
