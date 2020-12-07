#include "pgobjectwizard.h"
#include "ui_pgobjectwizard.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

pgObjectWizard::pgObjectWizard(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pgObjectWizard)
{
    ui->setupUi(this);
    StartInit();
}

pgObjectWizard::~pgObjectWizard()
{
    delete ui;
}

int pgObjectWizard::getObjectID() const
{
    return objectID;
}

void pgObjectWizard::RefreshObjectPage()
{
    SwitchObjectMode(ObjectMode::Choose);
}

void pgObjectWizard::usr_ObjectIndex_changed(int index)
{
    m_ObjectMapper->setCurrentIndex(index);

    if (index == -1) {
        objectID = index;
    }else {
        objectID = m_ObjectModel->data(m_ObjectModel->index(index, 0), Qt::DisplayRole).toInt();
    }
    emit usr_ObjectId_changed(objectID);
}

void pgObjectWizard::usr_ObjectText_changed(const QString &text)
{
    Q_UNUSED(text)
    if ((ui->txt_Address->toPlainText() == "") || (ui->ed_Telephon->text() == "")) {
        ui->pb_AddObject->setEnabled(false);
    }else {
        ui->pb_AddObject->setEnabled(true);
    }
}

void pgObjectWizard::SwitchObjectMode(pgObjectWizard::ObjectMode mode)
{
    if (mode == ObjectMode::Add) {
        runObjectMode = ObjectMode::Add;
        this->setSubTitle("Введите данные нового объекта");
        ui->pb_AddObject->setHidden(false);
        ui->pb_AddObject->setEnabled(false);
        ui->cb_Object->setEditable(true);
        ui->cb_Object->setCurrentIndex(-1);
        ui->cb_Object->setCurrentText("");
        QObject::connect(ui->cb_Object->lineEdit(), SIGNAL(textChanged(const QString)), this, SLOT(usr_ObjectText_changed(const QString)));
        ui->txt_Address->setReadOnly(false);
        ui->txt_Address->setText("");
        ui->cb_Object->lineEdit()->setPlaceholderText("Введите наименование объекта");
        ui->txt_Address->setPlaceholderText("Введите адрес объекта");
        ui->pb_Switch->setIcon(QPixmap(":/icons/Icons/Error.ico"));
        ui->ed_Telephon->setReadOnly(false);
        ui->ed_Telephon->setText("");
        QObject::connect(ui->ed_Telephon, SIGNAL(textChanged(const QString)), this, SLOT(usr_ObjectText_changed(const QString)));
        ui->ed_Email->setReadOnly(false);
        ui->ed_Email->setText("");
        ui->ed_Person->setReadOnly(false);
        ui->ed_Person->setText("");
    }else {
        runObjectMode = ObjectMode::Choose;
        this->setSubTitle("Выберете объект из базы данных");
        if (ui->cb_Object->lineEdit() != nullptr) {
            QObject::disconnect(ui->cb_Object->lineEdit(), SIGNAL(textChanged(const QString)), this, SLOT(usr_ObjectText_changed(const QString)));
        }
        ui->pb_AddObject->setHidden(true);
        ui->cb_Object->setEditable(false);
        ui->cb_Object->setCurrentIndex(-1);
        ui->txt_Address->setReadOnly(true);
        ui->txt_Address->setText("");
        ui->txt_Address->setPlaceholderText("");
        ui->pb_Switch->setIcon(QPixmap(":/icons/Icons/register.ico"));
        ui->ed_Telephon->setReadOnly(true);
        ui->ed_Telephon->setText("");
        QObject::disconnect(ui->ed_Telephon, SIGNAL(textChanged(const QString)), this, SLOT(usr_ObjectText_changed(const QString)));
        ui->ed_Email->setReadOnly(true);
        ui->ed_Email->setText("");
        ui->ed_Person->setReadOnly(true);
        ui->ed_Person->setText("");
    }
}

void pgObjectWizard::StartInit()
{
    ui->pb_AddObject->setHidden(true);
    ui->pb_AddObject->setEnabled(false);

    SwitchObjectMode(ObjectMode::Choose);

    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};

    m_ObjectModel = new QSqlQueryModel(this);
    m_ObjectMapper = new QDataWidgetMapper(this);
    m_ObjectModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Object));
    delete m_DBProcessor;

    QObject::connect(ui->cb_Object, SIGNAL(currentIndexChanged(int)), this, SLOT(usr_ObjectIndex_changed(int)));

    ui->cb_Object->setModel(m_ObjectModel);
    ui->cb_Object->setModelColumn(1);
    m_ObjectMapper->setModel(m_ObjectModel);
    m_ObjectMapper->addMapping(ui->txt_Address, 2);
    m_ObjectMapper->addMapping(ui->ed_Person, 4);
    m_ObjectMapper->addMapping(ui->ed_Telephon, 5);
    m_ObjectMapper->addMapping(ui->ed_Email, 6);
    ui->cb_Object->setCurrentIndex(-1);

    this->registerField("objectID*", ui->cb_Object);
}

void pgObjectWizard::on_pb_AddObject_clicked()
{
    QSortFilterProxyModel *m_CheckProxy {new QSortFilterProxyModel(this)};
    int lastID {-1};
    m_CheckProxy->setSourceModel(m_ObjectModel);
    m_CheckProxy->setFilterKeyColumn(1);
    m_CheckProxy->setFilterRegExp("^\\" + ui->cb_Object->lineEdit()->text() + "&");
    if (m_CheckProxy->rowCount() > 0) {
        QMessageBox m_MsgBox(QMessageBox::Warning, "Внимание!", "Данный объект уже существует", QMessageBox::Ok, this);
        m_MsgBox.exec();
    }else {
        db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
        QVariantList sendArgs {};
        sendArgs << ui->cb_Object->lineEdit()->text() << ui->txt_Address->toPlainText() << ui->ed_Person->text() << ui->ed_Telephon->text() << ui->ed_Email->text();
        lastID = m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Object);
        delete m_ReqInserter;
        if (lastID == -1) {
            QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось добавить объект", QMessageBox::Ok, this);
            m_MsgBox.exec();
        }else {
            QMessageBox m_MsgBox(QMessageBox::Information, "Завершено", "Объект успешно добавлен", QMessageBox::Ok, this);
            m_MsgBox.exec();
            m_ObjectModel->setQuery(m_ObjectModel->query().lastQuery());
            SwitchObjectMode(ObjectMode::Choose);
            m_CheckProxy->setFilterKeyColumn(0);
            m_CheckProxy->setFilterRegExp(QString::number(lastID));
            int objRow {m_CheckProxy->mapToSource(m_CheckProxy->index(0,0)).row()};
            ui->cb_Object->setCurrentIndex(objRow);
        }
        delete m_CheckProxy;
    }
}

void pgObjectWizard::on_pb_Switch_clicked()
{
    if (runObjectMode == ObjectMode::Add) {
        runObjectMode = ObjectMode::Choose;
    }else {
        runObjectMode = ObjectMode::Add;
    }
    SwitchObjectMode(runObjectMode);
}
