#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->label_loggingin->hide();
    ui->label_status_login->setText("Connecting to server");
    initializeDataBase();

    connect(ui->radioButton_teacher, SIGNAL(clicked(bool)), this, SLOT(ifRadioButton()));
    connect(ui->radioButton_student, SIGNAL(clicked(bool)), this, SLOT(ifRadioButton()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void LoginDialog::initializeDataBase()
{
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("mydatabase.sqlite");
        if (!db.open()) {
            ui->label_status_login->setText("Unable to open the database");
        } else {
            ui->label_status_login->setText("Connected to Server");
            qry = new QSqlQuery(db);
        }
    } else {
        db = QSqlDatabase::database();
    }
}

void LoginDialog::on_pushButton_login_clicked()
{
    if (!db.isOpen()) {
        qDebug() << "Failed to open the database";
        return;
    }


    if (!ui->lineEdit_ID->text().isEmpty() || !ui->lineEdit_password->text().isEmpty()) {
        if (ui->radioButton_teacher->isChecked()) {
            if (qry->exec("select * from Teacher where ID='" + ui->lineEdit_ID->text() + "' and password='" + ui->lineEdit_password->text() + "'")) {
                int count = 0;
                while(qry->next())
                    count++;
                if (count == 1) {
                    ui->groupBox->hide();
                    ui->label_loggingin->show();

                    delay(1500);
                    Teacherinterface *std = new Teacherinterface(ui->lineEdit_ID->text(), ui->lineEdit_password->text(), db);
                    this->hide();
                    std->show();

                }
                if (count > 1) {
                    ui->groupBox->hide();
                    ui->label_loggingin->show();
                    delay(1500);
                    ui->label_loggingin->hide();
                    ui->groupBox->show();

                    ui->label_status_login->setText("Duplicate username or password is correct");
                }
                if (count < 1) {
                    ui->groupBox->hide();
                    ui->label_loggingin->show();
                    delay(1500);
                    ui->label_loggingin->hide();
                    ui->groupBox->show();

                    ui->label_status_login->setText("Teacher ID or password is not correct");
                }
            } else {
                ui->groupBox->hide();
                ui->label_loggingin->show();
                delay(1500);
                ui->label_loggingin->hide();
                ui->groupBox->show();

                ui->label_status_login->setText("Teacher ID or password is not correct");
            }
        }
        if (ui->radioButton_student->isChecked()) {
            if(qry->exec("select * from Student where ID='" + ui->lineEdit_ID->text() + "' and password='" + ui->lineEdit_password->text() + "'")) {
                int count = 0;
                while(qry->next()) {
                    count++;
                }
                if (count == 1) {
                    ui->groupBox->hide();
                    ui->label_loggingin->show();
                    delay(1500);

                    StudentInterface *std = new StudentInterface(ui->lineEdit_ID->text(), ui->lineEdit_password->text(), db);
                    this->hide();
                    std->show();
                }
                if (count > 1) {
                    ui->groupBox->hide();
                    ui->label_loggingin->show();
                    delay(1500);
                    ui->label_loggingin->hide();
                    ui->groupBox->show();

                    ui->label_status_login->setText("Duplicate Student ID or password is correct");
                }
                if (count < 1) {
                    ui->groupBox->hide();
                    ui->label_loggingin->show();
                    delay(1500);
                    ui->label_loggingin->hide();
                    ui->groupBox->show();

                    ui->label_status_login->setText("Student ID or password is not correct");
                }
            } else {
                ui->label_status_login->setText("Student ID or password is not correct");
            }
        }
    } else {
        ui->label_status_login->setText("Unable to continue. Empty field");
    }
    if(!ui->radioButton_student->isChecked() && !ui->radioButton_teacher->isChecked())
        ui->label_status_login->setText("Select Teacher or Student");
    if(ui->lineEdit_ID->text().isEmpty() && ui->lineEdit_password->text().isEmpty())
        ui->label_status_login->setText("Unable to continue. Empty field");
}

void LoginDialog::ifRadioButton()
{
    if (ui->radioButton_student->isChecked())
        ui->lineEdit_ID->setPlaceholderText("Student ID");
    if (ui->radioButton_teacher->isChecked())
        ui->lineEdit_ID->setPlaceholderText("Teacher ID");
}
