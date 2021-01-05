#ifndef CLEXELEXPORT_H
#define CLEXELEXPORT_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QAxObject;
QT_END_NAMESPACE

class clExelExport
{
public:
    clExelExport(const clExelExport& other) = delete;
    clExelExport& operator=(const clExelExport& other) = delete;
    clExelExport(bool closeExcelOnExit = false);
    ~clExelExport();

    void SetCellValue(int lineIndex, int columnIndex, const QVariant value);

private:
    QAxObject* m_excelApplication;
    QAxObject* m_workbooks;
    QAxObject* m_workbook;
    QAxObject* m_sheets;
    QAxObject* m_sheet;
    bool m_closeExcelOnExit;
};

#endif // CLEXELEXPORT_H
