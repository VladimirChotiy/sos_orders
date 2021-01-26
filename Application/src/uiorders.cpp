#include "uiorders.h"
#include "ui_uiorders.h"
#include "settings.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QLayout>
#include <QToolButton>
#include <QDateTime>
#include <QToolButton>
#include <QStandardPaths>

uiOrders::uiOrders(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiOrders),
    reqID(id)
{
    ui->setupUi(this);
    LoadDialogSettings();
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

    QMap<QHBoxLayout*, int>::iterator map_iterator = layoutsMap.begin();
    for (; map_iterator != layoutsMap.end(); ++map_iterator) {
        while (QLayoutItem *l_item = map_iterator.key()->takeAt(0)) {
            if (l_item->widget()){
                map_iterator.key()->removeWidget(l_item->widget());
                delete l_item->widget();
            }else {
                map_iterator.key()->removeItem(l_item);
                delete l_item;
            }
        }
        delete map_iterator.key();
    }
    layoutsMap.clear();
    buttonsMap.clear();

    while (QLayoutItem *docSpacer = ui->vl_DocumentLayout->takeAt(0)) {
        if (docSpacer->spacerItem()) {
            ui->vl_DocumentLayout->removeItem(docSpacer);
        }
    }

    GetOrdersQuery();
    while (ordersQuery.next()) {
        CreateLayout(ordersQuery.value(0).toInt(), ordersQuery.value(1).toString(), ordersQuery.value(4).toDateTime(), ordersQuery.value(2).toInt());
    }

    ui->vl_DocumentLayout->addStretch();
}

QHBoxLayout* uiOrders::CreateLayout(int id, const QString fileName, QDateTime date, int docType)
{
    QHBoxLayout *m_fileLayout = new QHBoxLayout;

    QLabel *m_icoLabel = new QLabel();
    switch (docType) {
    case 1:{
        m_icoLabel->setPixmap(QPixmap(":/icons/Icons/word_files.ico"));
        break;
    }
    case 2: {
        m_icoLabel->setPixmap(QPixmap(":/icons/Icons/excel_files.ico"));
        break;
    }
    case 3: {
        m_icoLabel->setPixmap(QPixmap(":/icons/Icons/pdf.ico"));
        break;
    }
    default: {
        m_icoLabel->setPixmap(QPixmap(":/icons/Icons/attached.ico"));
    }
    }
    m_icoLabel->setScaledContents(true);
    m_icoLabel->setMaximumSize(22, 22);
    m_fileLayout->addWidget(m_icoLabel);

    QLabel *m_fileLabel = new QLabel();
    m_fileLabel->setMaximumHeight(24);
    m_fileLabel->setTextFormat(Qt::RichText);
    m_fileLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_fileLabel->setOpenExternalLinks(true);
    m_fileLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    m_fileLabel->setFont(QFont("MS Shell Dlg 2", 10));
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

    QObject::connect(m_delButton, &QToolButton::clicked, [this, m_delButton](){
        this->deleteButton_clicked(m_delButton);
    });
    m_fileLayout->addWidget(m_delButton);

    layoutsMap.insert(m_fileLayout, id);
    buttonsMap.insert(m_delButton, id);

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

    QString lastLocation {};
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("OrdersDlg");
    m_settings->OpenGroup();
    lastLocation = m_settings->getParam("LastLocation").toString();
    m_settings->CloseGroup();

    if (!QDir(lastLocation).exists()) {
        lastLocation = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
    }

    srcPath = QFileDialog::getOpenFileName(this, "Добавить файл", lastLocation, "All Files (*.*);;Microsoft Word Files (*.doc *.docx);;Microsoft Excel Files (*.xls *.xlsx);;PDF Files (*.pdf)");
    QFile srcFile(srcPath);

    if (!srcFile.exists()) {
        return std::make_pair("", DocType::dtError);
    }

    m_settings->setGroupName("OrdersDlg");
    m_settings->OpenGroup();
    m_settings->setParam("LastLocation", QFileInfo(srcFile).absolutePath());
    m_settings->CloseGroup();
    delete m_settings;

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

void uiOrders::deleteButton_clicked(QToolButton *tButton)
{
    QString remFileName;
    ordersQuery.first();
    if (ordersQuery.value(0).toInt() == buttonsMap.value(tButton)) {
        remFileName = ordersQuery.value(1).toString();
    }else {
        while (ordersQuery.next()) {
            if (ordersQuery.value(0).toInt() == buttonsMap.value(tButton)) {
                remFileName = ordersQuery.value(1).toString();
                break;
            }
        }
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Внимание", "Вы дествительно хотите удалить файл " + remFileName + "?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if(QFile(homeDir + remFileName).remove()) {
            db::clDBReqInserter *m_DBInserter {new db::clDBReqInserter(this)};
            m_DBInserter->DeleteRecord(buttonsMap.value(tButton), DBTypes::DBRemoveType::Order);
            delete m_DBInserter;
        }else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить файл " + remFileName, QMessageBox::Ok);
        }
    }

    QStringList filesInfo = QDir(homeDir).entryList(QDir::Files|QDir::NoDotAndDotDot);
    if (filesInfo.isEmpty()) {
        if (!QDir(homeDir).rmdir(QDir(homeDir).absolutePath())) {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить каталог" + QDir(homeDir).path(), QMessageBox::Ok);
        }
    }

    RefreshFilesList();

}

void uiOrders::SaveDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("OrdersDlg");
    m_settings->OpenGroup();
    m_settings->setParam("Width", this->geometry().width());
    m_settings->setParam("Heigth", this->geometry().height());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiOrders::LoadDialogSettings()
{
    settings::StoreSettings *m_settings {new settings::StoreSettings()};
    m_settings->setGroupName("OrdersDlg");
    m_settings->OpenGroup();
    this->resize(m_settings->getParam("Width").toInt(), m_settings->getParam("Heigth").toInt());
    m_settings->CloseGroup();
    delete m_settings;
}

void uiOrders::on_uiOrders_accepted()
{
    SaveDialogSettings();
}
