#include "pgpersonwizard.h"
#include "ui_pgpersonwizard.h"
#include "DBProcessor.h"
#include "clDBReqInserter.h"
#include <QSortFilterProxyModel>
#include <QMessageBox>

pgPersonWizard::pgPersonWizard(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pgPersonWizard)
{
    ui->setupUi(this);
    StartInit();
}

pgPersonWizard::~pgPersonWizard()
{
    delete ui;
}

void pgPersonWizard::SwitchPersonMode(pgPersonWizard::PersonMode mode)
{
    if (mode == PersonMode::Add) {
        runPersonMode = PersonMode::Add;
        this->setSubTitle("Введите данные нового заказчика");
        ui->pb_SavePerson->setHidden(false);
        ui->pb_SavePerson->setEnabled(false);
        ui->cb_Person->setEditable(true);
        ui->cb_Person->setCurrentIndex(-1);
        ui->cb_Person->setCurrentText("");
        QObject::connect(ui->cb_Person->lineEdit(), SIGNAL(textChanged(const QString)), this, SLOT(usr_PersonText_changed(const QString)));
        ui->ed_Telephon->setReadOnly(false);
        ui->ed_Email->setReadOnly(false);
        ui->ed_Telephon->setText("");
        ui->ed_Email->setText("");
        ui->cb_Person->lineEdit()->setPlaceholderText("Введите Ф.И.О. заказчика");
        ui->ed_Telephon->setPlaceholderText("Введите номер телефона");
        ui->ed_Email->setPlaceholderText("Введите адрес электронной почты");
        ui->pb_AddPerson->setIcon(QPixmap(":/icons/Icons/Error.ico"));
    }else {
        runPersonMode = PersonMode::Choose;
        this->setSubTitle("Выберете заказчика из базы данных");
        if (ui->cb_Person->lineEdit() != nullptr){
            QObject::disconnect(ui->cb_Person->lineEdit(), SIGNAL(textChanged(const QString)), this, SLOT(usr_PersonText_changed(const QString)));
        }
        ui->pb_SavePerson->setHidden(true);
        ui->cb_Person->setEditable(false);
        ui->cb_Person->setCurrentIndex(-1);
        ui->ed_Telephon->setReadOnly(true);
        ui->ed_Email->setReadOnly(true);
        ui->ed_Telephon->setText("");
        ui->ed_Email->setText("");
        ui->ed_Telephon->setPlaceholderText("");
        ui->ed_Email->setPlaceholderText("");
        ui->pb_AddPerson->setIcon(QPixmap(":/icons/Icons/register.ico"));
    }
}

void pgPersonWizard::StartInit()
{
    ui->pb_SavePerson->setHidden(true);
    ui->pb_SavePerson->setEnabled(false);

    SwitchPersonMode(PersonMode::Choose);

    db::DBProcessor *m_DBProcessor {new db::DBProcessor()};

    m_PersonModel = new QSqlQueryModel(this);
    m_PersonMapper = new QDataWidgetMapper(this);
    m_PersonModel->setQuery(m_DBProcessor->prepareQuery(DBTypes::QueryType::Person));
    delete m_DBProcessor;

    ui->cb_Person->setModel(m_PersonModel);
    ui->cb_Person->setModelColumn(1);
    m_PersonMapper->setModel(m_PersonModel);
    m_PersonMapper->addMapping(ui->ed_Telephon, 2);
    m_PersonMapper->addMapping(ui->ed_Email, 3);
    QObject::connect(ui->cb_Person, SIGNAL(currentIndexChanged(int)), this, SLOT(usr_PersonIndex_changed(int)));
    ui->cb_Person->setCurrentIndex(-1);

    this->registerField("personID*", ui->cb_Person);
}

int pgPersonWizard::getPersonID() const
{
    return personID;
}

void pgPersonWizard::on_pb_AddPerson_clicked()
{
    if (runPersonMode == PersonMode::Add) {
        runPersonMode = PersonMode::Choose;
    }else {
        runPersonMode = PersonMode::Add;
    }
    SwitchPersonMode(runPersonMode);
}

void pgPersonWizard::usr_PersonIndex_changed(int index)
{
    m_PersonMapper->setCurrentIndex(index);
    if (index == -1) {
        personID = index;
    }else {
        personID = m_PersonModel->data(m_PersonModel->index(index, 0), Qt::DisplayRole).toInt();
    }
    emit usr_PersonId_changed(personID);
}

void pgPersonWizard::usr_PersonText_changed(const QString &text)
{
    if (text == "") {
        ui->pb_SavePerson->setEnabled(false);
    }else {
        ui->pb_SavePerson->setEnabled(true);
    }
}

void pgPersonWizard::on_pb_SavePerson_clicked()
{
    QSortFilterProxyModel *m_PersonProxy {new QSortFilterProxyModel(this)};
    int lastID;
    m_PersonProxy->setSourceModel(m_PersonModel);
    m_PersonProxy->setFilterKeyColumn(1);
    m_PersonProxy->setFilterRegExp("^" + ui->cb_Person->lineEdit()->text() + "&");
    if (m_PersonProxy->rowCount() > 0) {
        QMessageBox m_MsgBox(QMessageBox::Warning, "Внимание!", "Данный пользователь уже существует в базе данных", QMessageBox::Ok, this);
        m_MsgBox.exec();
    }else {
        db::clDBReqInserter *m_ReqInserter {new db::clDBReqInserter(this)};
        QVariantList sendArgs {};
        sendArgs << ui->cb_Person->lineEdit()->text() << ui->ed_Telephon->text() << ui->ed_Email->text();
        lastID = m_ReqInserter->AddData(sendArgs, DBTypes::DBInsertType::Person);
        delete m_ReqInserter;
        if (lastID == -1) {
            QMessageBox m_MsgBox(QMessageBox::Critical, "Ошибка!", "Не удалось добавить пользователя", QMessageBox::Ok, this);
            m_MsgBox.exec();
        }else {
            QMessageBox m_MsgBox(QMessageBox::Information, "Завершено", "Пользователь успешно добавлен", QMessageBox::Ok, this);
            m_MsgBox.exec();
            m_PersonModel->setQuery(m_PersonModel->query().lastQuery());
            SwitchPersonMode(PersonMode::Choose);
            m_PersonProxy->setFilterKeyColumn(0);
            m_PersonProxy->setFilterRegExp(QString::number(lastID));
            int prsRow {m_PersonProxy->mapToSource(m_PersonProxy->index(0,0)).row()};
            ui->cb_Person->setCurrentIndex(prsRow);
        }
    }
    delete m_PersonProxy;
}
