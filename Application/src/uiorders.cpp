#include "uiorders.h"
#include "ui_uiorders.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QLayout>
#include <QToolButton>
#include <QDateTime>

uiOrders::uiOrders(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiOrders),
    reqID(id)
{
    ui->setupUi(this);
    ui->sa_ScrollFrame->setBackgroundRole(QPalette::Dark);
    layoutsMap.clear();

    homeDir = "//fs1.corp.sos-dn.net/DocShare/RequestFiles/" + QString::number(reqID) + "/";

    RefreshFilesList();
}

uiOrders::~uiOrders()
{
    delete ui;
}

void uiOrders::on_pb_AddOrder_clicked()
{
    QString fName;
    DocType dType;
    std::tie(fName, dType) = AttachFile();

    if (dType != DocType::dtError) {
        UpdateDBInfo(fName, dType);
    }
}

void uiOrders::RefreshFilesList()
{
    GetOrdersQuery();
    layoutsMap.clear();
    while (ordersQuery.next()) {
        layoutsMap.insert(CreateLayout(ordersQuery.value(1).toString(), ordersQuery.value(4).toDateTime(), ordersQuery.value(2).toInt()), ordersQuery.value(0).toInt());
    }

    for (auto m_Map : layoutsMap.keys()){
        qDebug() << m_Map << " : " << layoutsMap.value(m_Map);
    }

    ui->vl_DocumentLayout->addStretch();
}

QHBoxLayout* uiOrders::CreateLayout(const QString fileName, QDateTime date, int docType)
{
    QHBoxLayout *m_fileLayout = new QHBoxLayout;

    QLabel *m_icoLabel = new QLabel();
    m_icoLabel->setPixmap(QPixmap(":/icons/Icons/attached.ico"));
    m_icoLabel->setScaledContents(true);
    m_icoLabel->setMaximumSize(22, 22);
    m_fileLayout->addWidget(m_icoLabel);

    QLabel *m_fileLabel = new QLabel();
    m_fileLabel->setMaximumHeight(24);
    m_fileLabel->setTextFormat(Qt::RichText);
    m_fileLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_fileLabel->setOpenExternalLinks(true);
    m_fileLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    m_fileLabel->setText("<a href='file://" + homeDir + fileName + "'>" + fileName + "</a>");
    m_fileLayout->addWidget(m_fileLabel);

    m_fileLayout->addStretch();

    QLabel *m_dateLabel = new QLabel();
    //m_dateLabel->setTextFormat(Qt::RichText);
    m_dateLabel->setFont(QFont("MS Shell Dlg 2", 9));
    m_dateLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_dateLabel->setText("<font color='black'>" + date.toString("dd-MM-yyyy HH:mm") + "</font>");
    m_fileLayout->addWidget(m_dateLabel);

    QToolButton *m_delButton = new QToolButton(this);
    m_delButton->setMaximumSize(24, 24);
    m_delButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_delButton->setIcon(QIcon(":/icons/Icons/Error.ico"));
    m_fileLayout->addWidget(m_delButton);

    ui->vl_DocumentLayout->addLayout(m_fileLayout);

    return m_fileLayout;
}

std::pair<QString, uiOrders::DocType> uiOrders::AttachFile()
{
    QString srcPath;
    QString dstPath;

    QDir reqDir(homeDir);
    if (!reqDir.exists()){
        if (!reqDir.mkpath(".")){
            QMessageBox::critical(this, "Ошибка!", "Невозможно создать каталог на сервере", QMessageBox::Ok);
            return std::make_pair("", DocType::dtError);
        }
    }

    srcPath = QFileDialog::getOpenFileName(this, "Добавить файл", QDir::currentPath(), "All Files (*.*);;Microsoft Word Files (*.doc *.docx);;Microsoft Excel Files (*.xls *.xlsx);;PDF Files (*.pdf)");
    QFile srcFile(srcPath);
    dstPath = homeDir + QFileInfo(srcFile).fileName();
    QFile dstFile(dstPath);
    if (dstFile.exists()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Внимание!", "Файл " + QFileInfo(dstFile).fileName() + " существует. Перезаписать?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No){
            return std::make_pair("", DocType::dtError);
        }else {
            if(!dstFile.remove()){
                QMessageBox::critical(this, "Ошибка!", "Невозможно перезаписать файл " + QFileInfo(dstFile).fileName(), QMessageBox::Ok);
                return std::make_pair("", DocType::dtError);
            }
        }
    }
    if (!QFile::copy(srcPath, dstPath)) {
        QMessageBox::critical(this, "Ошибка!", "Не удалось скопировать файл", QMessageBox::Ok);
        return std::make_pair("", DocType::dtError);
    }
    QString fileType = QFileInfo(srcFile).fileName();
    if ((fileType.contains(".doc", Qt::CaseInsensitive)) || (fileType.contains(".docx", Qt::CaseInsensitive))) {
        return std::make_pair(fileType, DocType::dtMSWord);
    }else if ((fileType.contains(".xls", Qt::CaseInsensitive)) || (fileType.contains(".xlsx", Qt::CaseInsensitive))) {
        return std::make_pair(fileType, DocType::dtMSExcel);
    }else if (fileType.contains(".pdf", Qt::CaseInsensitive)) {
        return std::make_pair(fileType, DocType::dtPdf);
    }else {
        return std::make_pair(fileType, DocType::dtDefault);
    }
}

bool uiOrders::UpdateDBInfo(const QString fileName, uiOrders::DocType docType)
{
    int updateID {-1};
    GetOrdersQuery();
    while (ordersQuery.next()) {
        if (ordersQuery.value(1).toString() == fileName) {
            updateID = ordersQuery.value(0).toInt();
            break;
        }
    }

    int dType {0};
    switch (docType) {
    case DocType::dtMSWord: {
        dType = 1;
        break;
    }
    case DocType::dtMSExcel: {
        dType = 2;
        break;
    }
    case DocType::dtPdf: {
        dType = 3;
        break;
    }
    default: {
        dType = 0;
        break;
    }
    }
    QVariantList sendArgs {};
    sendArgs << fileName << dType << reqID;

    db::clDBReqInserter *m_DBInserter {new db::clDBReqInserter(this)};

    if (updateID == -1) {
        m_DBInserter->AddData(sendArgs, DBTypes::DBInsertType::Order);
    }else {
        m_DBInserter->UpdateData(updateID, sendArgs, DBTypes::DBUpdateType::Order);
    }
    delete m_DBInserter;

    RefreshFilesList();

    return true;
}

void uiOrders::GetOrdersQuery()
{
    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};
    ordersQuery = m_DBProcessor->prepareQuery(DBTypes::QueryType::Order, reqID);
    delete m_DBProcessor;
}

void uiOrders::ClearLayout(QLayout *layout)
{

}
