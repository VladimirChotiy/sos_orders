#ifndef UISTATUSFILTER_H
#define UISTATUSFILTER_H

#include <QDialog>

namespace Ui {
class uiStatusFilter;
}

class uiStatusFilter : public QDialog
{
    Q_OBJECT

public:
    explicit uiStatusFilter(QWidget *parent = nullptr);
    ~uiStatusFilter();

private slots:
    void on_uiStatusFilter_accepted();

    void on_uiStatusFilter_rejected();

private:
    Ui::uiStatusFilter *ui;
};

#endif // UISTATUSFILTER_H
