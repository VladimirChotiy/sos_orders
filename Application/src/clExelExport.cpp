#include "clExelExport.h"
#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>
#include <stdexcept>

//using namespace std;


clExelExport::clExelExport(bool closeExcelOnExit) :
    m_closeExcelOnExit(closeExcelOnExit)
{
    m_excelApplication = nullptr;
    m_sheet = nullptr;
    m_sheets = nullptr;
    m_workbook = nullptr;
    m_workbooks = nullptr;
    m_excelApplication = nullptr;

    m_excelApplication = new QAxObject("Excel.Application", nullptr);
    if (m_excelApplication == nullptr) {
        throw std::invalid_argument("Failed to initialize interop with Excel (probably Excel is not installed)");
    }

    m_excelApplication->dynamicCall("SetVisible(bool)", false);
    m_excelApplication->setProperty("DisplayAlerts", 0);

    m_workbooks = m_excelApplication->querySubObject("Workbooks");
    m_workbook = m_workbooks->querySubObject("Add");
    m_sheets = m_workbook->querySubObject("Worksheets");
    m_sheet = m_sheets->querySubObject("Item(1)");
    m_sheet->dynamicCall("Select()");
}

clExelExport::~clExelExport()
{
    if (m_excelApplication != nullptr)
    {
        if (!m_closeExcelOnExit)
        {
            m_excelApplication->setProperty("DisplayAlerts", 1);
            m_excelApplication->dynamicCall("SetVisible(bool)", true );
        }

        if (m_workbook != nullptr && m_closeExcelOnExit)
        {
            m_workbook->dynamicCall("Close (Boolean)", true);
            m_excelApplication->dynamicCall("Quit (void)");
        }
    }

    delete m_sheet;
    delete m_sheets;
    delete m_workbook;
    delete m_workbooks;
    delete m_excelApplication;
}

void clExelExport::SetCellValue(int lineIndex, int columnIndex, const QVariant value)
{
    QAxObject *cell {m_sheet->querySubObject("Cells(int,int)", lineIndex, columnIndex)};
    cell->setProperty("Value", QVariant(value));
    delete cell;
}
