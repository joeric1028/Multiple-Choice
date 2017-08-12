#include "teacherinterface.h"
#include "ui_teacherinterface.h"

Teacherinterface::Teacherinterface(const QString &TempID, const QString &TempPassword, QSqlDatabase &db) :
    ui(new Ui::Teacherinterface)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    db = QSqlDatabase::database();
    qry = new QSqlQuery(db);
    model = new QSqlQueryModel(this);
    tableModel = new QSqlTableModel(this, db);
    tempId = TempID;
    tempPassword = TempPassword;
    qDebug() << "Received Data! From LoginDialog to TeacherInterface:" << tempId << tempPassword;

    qry->prepare("select * from teacher where ID=:tempId and Password=:tempPassword;");
    qry->bindValue(":tempId", tempId);
    qry->bindValue(":tempPassword", tempPassword);
    if(qry->exec()) {
        qDebug() << "Teacher Account: Retrieve Successfully" << qry->lastError();
        while(qry->next()) {
            ui->label_welcome->setText("Welcome, " + qry->value("FirstName").toString() + " " + qry->value("MiddleName").toString() + " " + qry->value("LastName").toString() + "!");
        }
    } else {
        qDebug() << "Teacher Account: Retrieve Failed" << qry->lastError() << qry->lastQuery();
    }
}

Teacherinterface::~Teacherinterface()
{
    delete ui;
}

void Teacherinterface::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Teacherinterface::on_actionExit_triggered()
{
    exit(0);
}

void Teacherinterface::on_actionLogout_triggered()
{
    this->close();
    LoginDialog std;
    std.exec();
}

void Teacherinterface::on_actionCreate_Teacher_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_status_createTeacher->clear();
}

void Teacherinterface::on_actionWelcome_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);

    qry->prepare("select * from teacher where ID=:tempId and Password=:tempPassword;");
    qry->bindValue(":tempId", tempId);
    qry->bindValue(":tempPassword", tempPassword);
    qry->exec();
    while(qry->next()) {
        ui->label_welcome->setText("Welcome, " + qry->value("FirstName").toString() + " " + qry->value("MiddleName").toString() + " " + qry->value("LastName").toString() + "!" );
    }
}

void Teacherinterface::on_actionAbout_triggered()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void Teacherinterface::on_actionCreate_Student_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Teacherinterface::on_actionMy_Info_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);

    qry->prepare("select * from Teacher where ID=:tempId and Password=:tempPassword;");
    qry->bindValue(":tempId", tempId);
    qry->bindValue(":tempPassword", tempPassword);

    if (qry->exec()) {
        qDebug() << "My Info Updated: " << qry->lastError();
        while(qry->next()) {
            ui->label_teacherID_information_myInfo->setText(qry->value("ID").toString());
            ui->label_password_information_myInfo->setText("*****");
            ui->label_firstName_information_myInfo->setText(qry->value("FirstName").toString());
            ui->label_middleName_information_myInfo->setText(qry->value("MiddleName").toString());
            ui->label_lastName_information_myInfo->setText(qry->value("LastName").toString());
            ui->label_contactNo_information_myInfo->setText(qry->value("ContactNo").toString());
        }
    } else {
        qDebug() << "My Info Error: " <<qry->lastError();
    }
}

void Teacherinterface::on_pushButton_edit_information_myInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->label_status_edit_myInfo->clear();

    qry->prepare("select * from Teacher where ID=:tempId and Password=:tempPassword;");
    qry->bindValue(":tempId", tempId);
    qry->bindValue(":tempPassword", tempPassword);
    qry->exec();
    qDebug() << qry->lastError();
    while(qry->next()) {
        ui->lineEdit_teacherID_edit_myInfo->setText(qry->value("ID").toString());
        ui->lineEdit_password_edit_myInfo->setText(qry->value("Password").toString());
        ui->lineEdit_firstName_edit_myInfo->setText(qry->value("FirstName").toString());
        ui->lineEdit_middleName_edit_myInfo->setText(qry->value("MiddleName").toString());
        ui->lineEdit_lastName_edit_myInfo->setText(qry->value("LastName").toString());
        ui->lineEdit_contactNo_edit_myInfo->setText(qry->value("ContactNo").toString());
    }
}

void Teacherinterface::on_pushButton_update_edit_myInfo_clicked()
{
    qry->prepare("update Teacher set ID=:id, Password=:Pass, FirstName=:first, MiddleName=:middle, LastName=:last, ContactNo=:No where ID=:tempId and Password=:tempPassword;");
    qry->bindValue(":tempId", tempId);
    qry->bindValue(":tempPassword", tempPassword);
    qry->bindValue(":id", ui->lineEdit_teacherID_edit_myInfo->text());
    qry->bindValue(":Pass", ui->lineEdit_password_edit_myInfo->text());
    qry->bindValue(":first", ui->lineEdit_firstName_edit_myInfo->text());
    qry->bindValue(":middle", ui->lineEdit_middleName_edit_myInfo->text());
    qry->bindValue(":last", ui->lineEdit_lastName_edit_myInfo->text());
    qry->bindValue(":No", ui->lineEdit_contactNo_edit_myInfo->text());
    qry->exec();
    qDebug() << "Updating MyInfo: " << qry->lastError();
    if(!qry->isValid()) {
        ui->label_status_edit_myInfo->setText("Account updated");
        ui->pushButton_update_edit_myInfo->setDisabled(true);
        ui->pushButton_cancel_edit_myInfo->setDisabled(true);
        ui->lineEdit_teacherID_edit_myInfo->setDisabled(true);
        ui->lineEdit_password_edit_myInfo->setDisabled(true);
        ui->lineEdit_firstName_edit_myInfo->setDisabled(true);
        ui->lineEdit_middleName_edit_myInfo->setDisabled(true);
        ui->lineEdit_lastName_edit_myInfo->setDisabled(true);
        ui->lineEdit_contactNo_edit_myInfo->setDisabled(true);
        delay(1500);
        ui->pushButton_update_edit_myInfo->setDisabled(false);
        ui->pushButton_cancel_edit_myInfo->setDisabled(false);
        ui->lineEdit_teacherID_edit_myInfo->setDisabled(false);
        ui->lineEdit_password_edit_myInfo->setDisabled(false);
        ui->lineEdit_firstName_edit_myInfo->setDisabled(false);
        ui->lineEdit_middleName_edit_myInfo->setDisabled(false);
        ui->lineEdit_lastName_edit_myInfo->setDisabled(false);
        ui->lineEdit_contactNo_edit_myInfo->setDisabled(false);
        on_actionMy_Info_triggered();

    } else {
        ui->label_status_edit_myInfo->setText("Error");
    }
}

void Teacherinterface::on_pushButton_cancel_edit_myInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    ui->label_status_edit_myInfo->setText("Editing Account Canceled");
    delay(1500);
    ui->label_status_edit_myInfo->clear();
}

void Teacherinterface::on_pushButton_cancel_createTeacher_clicked()
{
    ui->lineEdit_teacherID_createTeacher->clear();
    ui->lineEdit_password_createTeacher->clear();
    ui->lineEdit_firstName_createTeacher->clear();
    ui->lineEdit_middleName_createTeacher->clear();
    ui->lineEdit_lastName_createTeacher->clear();
    ui->lineEdit_contactNo_createTeacher->clear();
    ui->label_status_createTeacher->setText("Teacher Account Canceled");
    delay(1500);
    ui->label_status_createTeacher->clear();
}

void Teacherinterface::on_pushButton_createAccount_createTeacher_clicked()
{
    if(ui->lineEdit_teacherID_createTeacher->text().isEmpty() || ui->lineEdit_password_createTeacher->text().isEmpty() || ui->lineEdit_firstName_createTeacher->text().isEmpty() || ui->lineEdit_middleName_createTeacher->text().isEmpty() || ui->lineEdit_lastName_createTeacher->text().isEmpty() || ui->lineEdit_contactNo_createTeacher->text().isEmpty()) {
        ui->label_status_createTeacher->setText("Required Field is Empty");
    } else {
        qry->prepare("select * from Teacher where ID=:TeacherID;");
        qry->bindValue(":TeacherID", ui->lineEdit_teacherID_createTeacher->text());
        if(qry->exec()) {
            int count = 0;
            while(qry->next()) {
                count++;
            }
            if(count == 1) {
                ui->label_status_createTeacher->setText("Teacher ID Already Created");
            }
            if(count < 1) {
                qry->prepare("insert into Teacher"
                             "(ID, Password, FirstName, MiddleName, LastName, ContactNo)"
                             "values (:teacherID, :Password, :FirstName, :MiddleName, :LastName, :ContactNo)");
                qry->bindValue(":teacherID", ui->lineEdit_teacherID_createTeacher->text());
                qry->bindValue(":Password", ui->lineEdit_password_createTeacher->text());
                qry->bindValue(":FirstName", ui->lineEdit_firstName_createTeacher->text());
                qry->bindValue(":MiddleName", ui->lineEdit_middleName_createTeacher->text());
                qry->bindValue(":LastName", ui->lineEdit_lastName_createTeacher->text());
                qry->bindValue(":ContactNo", ui->lineEdit_contactNo_createTeacher->text());
                qry->exec();

                ui->label_status_createTeacher->setText("Teacher Account Created");
                qDebug() << "Teacher Account:" << qry->lastError();

                qry->prepare("Create Table '" + ui->lineEdit_teacherID_createTeacher->text() + "ClassList' "
                             "('Subject' VARCHAR PRIMARY KEY, 'Room' VARCHAR, 'TimeLimit' VARCHAR);");
                if(qry->exec()) {
                    qDebug() << "Teacher Account: ClassList Created" << qry->lastError();
                } else {
                    qDebug() << "Teacher Account: ClassList Error" << qry->lastError();
                }
                ui->lineEdit_teacherID_createTeacher->clear();
                ui->lineEdit_password_createTeacher->clear();
                ui->lineEdit_firstName_createTeacher->clear();
                ui->lineEdit_middleName_createTeacher->clear();
                ui->lineEdit_lastName_createTeacher->clear();
                ui->lineEdit_contactNo_createTeacher->clear();
                delay(1500);
                ui->label_status_createTeacher->clear();
            }
        } else {
            qDebug() << "Student Account: error " << qry->lastError();
        }
    }
}


void Teacherinterface::on_pushButton_createAccount_createStudent_clicked()
{
    if(ui->lineEdit_studentID_createStudent->text().isEmpty() || ui->lineEdit_password_createStudent->text().isEmpty() || ui->lineEdit_firstName_createStudent->text().isEmpty() || ui->lineEdit_middleName_createStudent->text().isEmpty() || ui->lineEdit_lastName_createStudent->text().isEmpty() || ui->lineEdit_contactNo_createStudent->text().isEmpty() || ui->lineEdit_currentLevel_createStudent->text().isEmpty() || ui->lineEdit_birthplace_createStudent->text().isEmpty() || ui->comboBox_gender_createStudent->currentText() == "Select Gender" || ui->lineEdit_citizenship_createStudent->text().isEmpty() || ui->lineEdit_civilStatus_createStudent->text().isEmpty()) {
        ui->label_status_createStudent->setText("Required Field is Empty");
    } else {
        qry->prepare("select * from Student where ID=:StudentID;");
        qry->bindValue(":StudentID", ui->lineEdit_studentID_createStudent->text());

        if (qry->exec()) {
            int count = 0;
            qDebug() << "Student list looked" << qry->lastError() << qry->lastQuery();
            while(qry->next()) {
                count++;
                qDebug() << qry->value("ID").toString();
            }
            if (count == 1) {
                ui->label_status_createStudent->setText("Student ID Already Created");
            }
            if (count < 1) {
                qry->prepare("insert into Student"
                             "(ID, Password, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus)"
                             "values (:StudentID, :Password, :FirstName, :MiddleName, :LastName, :ContactNo, :CurrentLevel, :Birthday, :Birthplace, :Gender, :Citizenship, :CivilStatus)");
                qry->bindValue(":StudentID", ui->lineEdit_studentID_createStudent->text());
                qry->bindValue(":Password", ui->lineEdit_password_createStudent->text());
                qry->bindValue(":FirstName", ui->lineEdit_firstName_createStudent->text());
                qry->bindValue(":MiddleName", ui->lineEdit_middleName_createStudent->text());
                qry->bindValue(":LastName", ui->lineEdit_lastName_createStudent->text());
                qry->bindValue(":ContactNo", ui->lineEdit_contactNo_createStudent->text());
                qry->bindValue(":CurrentLevel", ui->lineEdit_currentLevel_createStudent->text());
                qry->bindValue(":Birthday", ui->dateEdit_birthday_createStudent->text());
                qry->bindValue(":Birthplace", ui->lineEdit_birthplace_createStudent->text());
                qry->bindValue(":Gender", ui->comboBox_gender_createStudent->currentText());
                qry->bindValue(":Citizenship", ui->lineEdit_contactNo_createStudent->text());
                qry->bindValue(":CivilStatus", ui->lineEdit_civilStatus_createStudent->text());

                if (qry->exec()) {
                    ui->label_status_createStudent->setText("Student Account Created");
                    qDebug() << "Student Account:" << qry->lastError();

                    ui->lineEdit_studentID_createStudent->clear();
                    ui->lineEdit_password_createStudent->clear();
                    ui->lineEdit_firstName_createStudent->clear();
                    ui->lineEdit_middleName_createStudent->clear();
                    ui->lineEdit_lastName_createStudent->clear();
                    ui->lineEdit_contactNo_createStudent->clear();
                    ui->lineEdit_currentLevel_createStudent->clear();
                    ui->dateEdit_birthday_createStudent->date();
                    ui->lineEdit_birthplace_createStudent->clear();
                    ui->comboBox_gender_createStudent->setCurrentIndex(0);
                    ui->lineEdit_citizenship_createStudent->clear();
                    ui->lineEdit_civilStatus_createStudent->clear();
                    delay(1500);
                    ui->label_status_createStudent->clear();
                } else {
                    qDebug() << "Student Account: error " << qry->lastError() << qry->lastQuery();
                }
            }
        } else {
            qDebug() << "Student Account: error " << qry->lastError() << qry->lastQuery();
        }
    }
}
void Teacherinterface::on_pushButton_cancel_createStudent_clicked()
{
    ui->lineEdit_studentID_createStudent->clear();
    ui->lineEdit_password_createStudent->clear();
    ui->lineEdit_firstName_createStudent->clear();
    ui->lineEdit_middleName_createStudent->clear();
    ui->lineEdit_lastName_createStudent->clear();
    ui->lineEdit_contactNo_createStudent->clear();
    ui->lineEdit_currentLevel_createStudent->clear();
    ui->dateEdit_birthday_createStudent->date();
    ui->lineEdit_birthplace_createStudent->clear();
    ui->comboBox_gender_createStudent->setCurrentIndex(0);
    ui->lineEdit_citizenship_createStudent->clear();
    ui->lineEdit_civilStatus_createStudent->clear();
    ui->label_status_createStudent->setText("Student Account Canceled");
    delay(1500);
    ui->label_status_createStudent->clear();
}

void Teacherinterface::on_actionDelete_triggered()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->lineEdit_filterBar_delete->clear();
    QSqlQuery query;
    query.prepare("SELECT ID, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus FROM Teacher"
                  " UNION "
                  "SELECT ID, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus FROM Student order BY ID;");
    query.exec();
    qDebug() << "Reading Student & Teacher: " << query.lastError();
    model->setQuery(query);
    ui->tableView_delete->setModel(model);
    ui->label_status_delete->setText(" Records Found! ");
}

void Teacherinterface::on_actionView_Student_Profile_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->comboBox_selectStudentID_viewStudentProfile->clear();
    ui->comboBox_selectStudentID_viewStudentProfile->addItem("Select Student ID");
    qry->prepare("select ID from Student order by ID;");

    if(qry->exec()) {
        qDebug() << "View Student Profile Updated: " << qry->lastError();
        while(qry->next()) {
            ui->comboBox_selectStudentID_viewStudentProfile->addItem(qry->value("ID").toString());
        }
    } else {
        qDebug() << "View Student Profile Error: " << qry->lastError() << qry->lastQuery();

    }
}

void Teacherinterface::on_pushButton_delete_clicked()
{
    if(!tempdeleteId.isEmpty()) {
        qry->prepare("Delete from Student"
                     " where ID='" + tempdeleteId + "';");
        qry->exec();
        if(!qry->isValid()) {
            ui->label_status_delete->setText("Succefully Deleted Account");
            delay(1500);
            ui->label_status_delete->clear();
        }
        qDebug() << "Deleting from Student:" << qry->lastError() << qry->lastQuery();
        qry->prepare("Delete from Teacher "
                     "where ID='" + tempdeleteId + "';");
        qry->exec();
        qDebug() << "Deleting from Teacher:" << qry->lastError() << qry->lastQuery();
        qDebug() << " Temp DeleteID: " << tempdeleteId;
        if(!qry->isValid()) {
            ui->label_status_delete->setText("Succefully Deleted Account");
            delay(1500);
            ui->label_status_delete->clear();
        }
        delay(500);
        ui->lineEdit_filterBar_delete->clear();
    } else {
        ui->label_status_delete->setText("Use Filter Bar");
        delay(1500);
        ui->label_status_delete->clear();
    }
}

void Teacherinterface::on_lineEdit_filterBar_delete_textChanged(const QString &arg1)
{
    tempdeleteId = arg1;
    if(!arg1.isEmpty()) {
        QSqlQuery query;
        query.prepare("SELECT ID, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus FROM Teacher WHERE ID LIKE '" + arg1 + "%'"
                      " UNION "
                      "SELECT ID, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus FROM Student WHERE ID LIKE '" + arg1 + "%';");
        query.exec();
        qDebug() << "Reading Student & Teacher: " << query.lastError();
        model->setQuery(query);
        ui->tableView_delete->setModel(model);
    } else {
        QSqlQuery query;
        query.prepare("SELECT ID, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus FROM Teacher"
                      " UNION "
                      "SELECT ID, FirstName, MiddleName, LastName, ContactNo, CurrentLevel, Birthday, Birthplace, Gender, Citizenship, CivilStatus FROM Student ORDER BY ID;");
        query.exec();
        qDebug() << "Reading Student & Teacher when Empty String: " << query.lastError();
        model->setQuery(query);
        ui->tableView_delete->setModel(model);
    }
}

void Teacherinterface::on_comboBox_selectStudentID_viewStudentProfile_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Select Student ID") {
        ui->label_studentId_information_viewStudentProfile->clear();
        ui->label_password_information_viewStudentProfile->clear();
        ui->label_firstName_information_viewStudentProfile->clear();
        ui->label_middleName_information_viewStudentProfile->clear();
        ui->label_lastName_information_viewStudentProfile->clear();
        ui->label_contactNo_information_viewStudentProfile->clear();
        ui->label_currentLevel_information_viewStudentProfile->clear();
        ui->label_birthday_information_viewStudentProfile->clear();
        ui->label_birthplace_information_viewStudentProfile->clear();
        ui->label_gender_information_viewStudentProfile->clear();
        ui->label_citizenship_information_viewStudentProfile->clear();
        ui->label_civilStatus_information_viewStudentProfile->clear();
        ui->pushButton_edit_information_viewStudentProfile->setDisabled(true);
    } else {
        qry->prepare("select * from Student where ID=:tempId;");
        qry->bindValue(":tempId", arg1);

        if (qry->exec()) {
            qDebug() << "View Student Profile Updated: " << qry->lastError();
            ui->pushButton_edit_information_viewStudentProfile->setDisabled(false);
            while(qry->next()) {
                ui->label_studentId_information_viewStudentProfile->setText(qry->value("ID").toString());
                ui->label_password_information_viewStudentProfile->setText("*****");
                ui->label_firstName_information_viewStudentProfile->setText(qry->value("FirstName").toString());
                ui->label_middleName_information_viewStudentProfile->setText(qry->value("MiddleName").toString());
                ui->label_lastName_information_viewStudentProfile->setText(qry->value("LastName").toString());
                ui->label_contactNo_information_viewStudentProfile->setText(qry->value("ContactNo").toString());
                ui->label_currentLevel_information_viewStudentProfile->setText(qry->value("CurrentLevel").toString());
                ui->label_birthday_information_viewStudentProfile->setText(qry->value("Birthday").toString());
                ui->label_birthplace_information_viewStudentProfile->setText(qry->value("Birthplace").toString());
                ui->label_gender_information_viewStudentProfile->setText(qry->value("Gender").toString());
                ui->label_citizenship_information_viewStudentProfile->setText(qry->value("Citizenship").toString());
                ui->label_civilStatus_information_viewStudentProfile->setText(qry->value("CivilStatus").toString());
            }
        } else {
            qDebug() << "View Student Profile Error: " <<qry->lastError();
        }
    }
    if(arg1 == "") {
        ui->label_studentId_information_viewStudentProfile->clear();
        ui->label_password_information_viewStudentProfile->clear();
        ui->label_firstName_information_viewStudentProfile->clear();
        ui->label_middleName_information_viewStudentProfile->clear();
        ui->label_lastName_information_viewStudentProfile->clear();
        ui->label_contactNo_information_viewStudentProfile->clear();
        ui->label_currentLevel_information_viewStudentProfile->clear();
        ui->label_birthday_information_viewStudentProfile->clear();
        ui->label_birthplace_information_viewStudentProfile->clear();
        ui->label_gender_information_viewStudentProfile->clear();
        ui->label_citizenship_information_viewStudentProfile->clear();
        ui->label_civilStatus_information_viewStudentProfile->clear();
        ui->pushButton_edit_information_viewStudentProfile->setDisabled(true);
    }
}

void Teacherinterface::on_pushButton_edit_information_viewStudentProfile_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->label_status_edit_viewStudentProfile->clear();

    if(ui->comboBox_selectStudentID_viewStudentProfile->currentText() == "Select Student ID") {

    } else {
        qry->prepare("select * from Student where ID=:tempId;");
        qry->bindValue(":tempId", ui->comboBox_selectStudentID_viewStudentProfile->currentText());
        qry->exec();
        qDebug() << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_studentID_edit_viewStudentProfile->setText(qry->value("ID").toString());
            ui->label_edit_viewStudentProfile->setText("Edit Student Profile: " + qry->value("ID").toString());
            ui->lineEdit_password_edit_viewStudentProfile->setText(qry->value("Password").toString());
            ui->lineEdit_firstName_edit_viewStudentProfile->setText(qry->value("FirstName").toString());
            ui->lineEdit_middleName_edit_viewStudentProfile->setText(qry->value("MiddleName").toString());
            ui->lineEdit_lastName_edit_viewStudentProfile->setText(qry->value("LastName").toString());
            ui->lineEdit_contactNo_edit_viewStudentProfile->setText(qry->value("ContactNo").toString());
            ui->lineEdit_currentLevel_edit_viewStudentProfile->setText(qry->value("CurrentLevel").toString());
            ui->dateEdit_birthday_edit_viewStudentProfile->setDate(qry->value("Birthday").toDate());
            ui->lineEdit_birthplace_edit_viewStudentProfile->setText(qry->value("Birthplace").toString());
            if(qry->value("Gender").toString() == "Male") {
                ui->comboBox_gender_edit_viewStudentProfile->setCurrentIndex(2);
            } else {
                ui->comboBox_gender_edit_viewStudentProfile->setCurrentIndex(1);
            }
            ui->lineEdit_citizenship_edit_viewStudentProfile->setText(qry->value("Citizenship").toString());
            ui->lineEdit_civilStatus_edit_viewStudentProfile->setText(qry->value("CivilStatus").toString());
        }
    }
}

void Teacherinterface::on_pushButton_cancel_edit_viewStudentProfile_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void Teacherinterface::on_pushButton_update_edit_viewStudentProfile_clicked()
{
    qry->prepare("update Student set ID=:id, Password=:Pass, FirstName=:first, MiddleName=:middle, LastName=:last, ContactNo=:No, CurrentLevel=:Current, Birthday=:Birthday, Birthplace=:Birthplace, Gender=:Gender, Citizenship=:Citizenship, CivilStatus=:Civil where ID=:tempId;");
    qry->bindValue(":tempId", ui->comboBox_selectStudentID_viewStudentProfile->currentText());
    qry->bindValue(":id", ui->lineEdit_studentID_edit_viewStudentProfile->text());
    qry->bindValue(":Pass", ui->lineEdit_password_edit_viewStudentProfile->text());
    qry->bindValue(":first", ui->lineEdit_firstName_edit_viewStudentProfile->text());
    qry->bindValue(":middle", ui->lineEdit_middleName_edit_viewStudentProfile->text());
    qry->bindValue(":last", ui->lineEdit_lastName_edit_viewStudentProfile->text());
    qry->bindValue(":No", ui->lineEdit_contactNo_edit_viewStudentProfile->text());
    qry->bindValue(":Current", ui->lineEdit_currentLevel_edit_viewStudentProfile->text());
    qry->bindValue(":Birthday", ui->dateEdit_birthday_edit_viewStudentProfile->text());
    qry->bindValue(":Birthplace", ui->lineEdit_birthplace_edit_viewStudentProfile->text());
    qry->bindValue(":Gender", ui->comboBox_gender_edit_viewStudentProfile->currentText());
    qry->bindValue(":Citizenship", ui->lineEdit_contactNo_edit_viewStudentProfile->text());
    qry->bindValue(":Civil", ui->lineEdit_civilStatus_edit_viewStudentProfile->text());
    qry->exec();
    qDebug() << "Updating MyInfo: " << qry->lastError();

    if(!qry->isValid()) {
        ui->label_status_edit_viewStudentProfile->setText("Account updated");
        ui->pushButton_update_edit_viewStudentProfile->setDisabled(true);
        ui->pushButton_cancel_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_studentID_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_password_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_firstName_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_middleName_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_lastName_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_contactNo_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_currentLevel_edit_viewStudentProfile->setDisabled(true);
        ui->dateEdit_birthday_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_birthplace_edit_viewStudentProfile->setDisabled(true);
        ui->comboBox_gender_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_citizenship_edit_viewStudentProfile->setDisabled(true);
        ui->lineEdit_civilStatus_edit_viewStudentProfile->setDisabled(true);
        delay(1500);
        ui->pushButton_update_edit_viewStudentProfile->setDisabled(false);
        ui->pushButton_cancel_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_studentID_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_password_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_firstName_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_middleName_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_lastName_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_contactNo_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_currentLevel_edit_viewStudentProfile->setDisabled(false);
        ui->dateEdit_birthday_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_birthplace_edit_viewStudentProfile->setDisabled(false);
        ui->comboBox_gender_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_citizenship_edit_viewStudentProfile->setDisabled(false);
        ui->lineEdit_civilStatus_edit_viewStudentProfile->setDisabled(false);
        qry->prepare("select * from Student where ID=:tempId;");
        qry->bindValue(":tempId", ui->comboBox_selectStudentID_viewStudentProfile->currentText());

        if (qry->exec()) {
            qDebug() << "View Student Profile Updated: " << qry->lastError();
            ui->pushButton_edit_information_viewStudentProfile->setDisabled(false);
            while(qry->next()) {
                ui->label_studentId_information_viewStudentProfile->setText(qry->value("ID").toString());
                ui->label_password_information_viewStudentProfile->setText("*****");
                ui->label_firstName_information_viewStudentProfile->setText(qry->value("FirstName").toString());
                ui->label_middleName_information_viewStudentProfile->setText(qry->value("MiddleName").toString());
                ui->label_lastName_information_viewStudentProfile->setText(qry->value("LastName").toString());
                ui->label_contactNo_information_viewStudentProfile->setText(qry->value("ContactNo").toString());
                ui->label_currentLevel_information_viewStudentProfile->setText(qry->value("CurrentLevel").toString());
                ui->label_birthday_information_viewStudentProfile->setText(qry->value("Birthday").toString());
                ui->label_birthplace_information_viewStudentProfile->setText(qry->value("Birthplace").toString());
                ui->label_gender_information_viewStudentProfile->setText(qry->value("Gender").toString());
                ui->label_citizenship_information_viewStudentProfile->setText(qry->value("Citizenship").toString());
                ui->label_civilStatus_information_viewStudentProfile->setText(qry->value("CivilStatus").toString());
            }
            ui->stackedWidget->setCurrentIndex(5);
        } else {
            qDebug() << "View Student Profile Error: " <<qry->lastError();
        }
    } else {
        ui->label_status_edit_viewStudentProfile->setText("Error");
    }

}

void Teacherinterface::on_actionCreate_Class_triggered()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void Teacherinterface::on_actionView_Class_List_triggered()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->comboBox_subject_viewClassList->clear();
    ui->comboBox_subject_viewClassList->addItem("Select Subject");
    qry->prepare("select Subject from '" + tempId + "ClassList' order by Subject;");

    if(qry->exec()) {
        qDebug() << "View Class List Updated: " << qry->lastError() << qry->lastQuery();
        while(qry->next()) {
            ui->comboBox_subject_viewClassList->addItem(qry->value("Subject").toString());
        }
    } else {
        qDebug() << "View Class List Error: " << qry->lastError() << qry->lastQuery();
    }
}


void Teacherinterface::on_pushButton_create_createClass_clicked()
{
    if(ui->lineEdit_subject_createClass->text().isEmpty() || ui->lineEdit_room_createClass->text().isEmpty() || ui->lineEdit_timelimitmins_createClass->text().isEmpty()) {
        ui->label_status_createClass->setText("Required Field is Empty");
    } else {
        qry->prepare("select * from '"+ tempId +"ClassList' where Subject='"+ ui->lineEdit_subject_createClass->text() +"';");
        if(qry->exec()) {
            int count = 0;
            while(qry->next()) {
                count++;
            }
            if(count == 1) {
                ui->label_status_createClass->setText("Subject Already Created");
            }
            if(count < 1) {
                qry->prepare("insert into '"+ tempId +"ClassList'"
                             "(Subject, Room, TimeLimit) "
                             "values ('"+ui->lineEdit_subject_createClass->text()+"', '"+ui->lineEdit_room_createClass->text()+"', '"+ui->lineEdit_timelimitmins_createClass->text()+"')");
                if (qry->exec()) {
                    ui->label_status_createClass->setText("Class Created");
                    qDebug() << "Class Created:" << qry->lastError();

                    qry->prepare("Create Table '" + tempId + "Class" + ui->lineEdit_subject_createClass->text() + "Result' "
                                 "('StudentID' VARCHAR PRIMARY KEY, 'Score' VARCHAR);");
                    if(qry->exec()) {
                        qDebug() << "Teacher Account: Class Created" << qry->lastError();
                    } else {
                        qDebug() << "Teacher Account: Class Error" << qry->lastError() << qry->lastQuery();
                    }

                    qry->prepare("Create Table '" + tempId + "Class" + ui->lineEdit_subject_createClass->text() + "MultipleChoice' "
                                 "('No' VARCHAR,'Questions' VARCHAR, 'Answer' VARCHAR, 'A' VARCHAR, 'B' VARCHAR, 'C' VARCHAR, 'D' VARCHAR);");
                    if(qry->exec()) {
                        qDebug() << "Teacher Account: Class Multiple Choice Created" << qry->lastError();
                        for(int count = 1; count < 11; count++) {
                            qry->prepare("insert into '"+ tempId + "Class"+ ui->lineEdit_subject_createClass->text() +"MultipleChoice' "
                                         "(No) "
                                         "values('"+QString::number(count)+"')");
                            if(qry->exec()) {
                                qDebug() << "Teacher Account: Class Multiple Choice Updated" << qry->lastError() << qry->lastQuery();
                            } else {
                                qDebug() << "Teacher Account: Class Multiple Choice Error" << qry->lastError() << qry->lastQuery();
                            }
                        }
                    } else {
                        qDebug() << "Teacher Account: Class Multiple Choice Error" << qry->lastError() << qry->lastQuery();
                    }
                    ui->lineEdit_subject_createClass->clear();
                    ui->lineEdit_room_createClass->clear();
                    delay(1500);
                    ui->label_status_createClass->clear();
                } else {
                    qDebug() << "Class Error:" << qry->lastError() << qry->lastQuery();
                }
            }
        } else {
            qDebug() << "Teacher Account: Class Error" << qry->lastError() << qry->lastQuery();
        }
    }
}

void Teacherinterface::on_pushButton_cancel_createClass_clicked()
{
    ui->lineEdit_subject_createClass->clear();
    ui->lineEdit_room_createClass->clear();

    ui->label_status_createClass->setText("Creating Class Canceled");
    delay(1500);
    ui->label_status_createClass->clear();
}

void Teacherinterface::on_comboBox_subject_viewClassList_currentIndexChanged(const QString &arg1)
{
    qDebug() << "arg1 = " << arg1;
    ui->tableView_viewClassList->hide();
    if(arg1 == "Select Subject") {
        ui->pushButton_editQuestions_viewClassList->setDisabled(true);
        ui->pushButton_results_viewClassList->setDisabled(true);
        ui->pushButton_questions->setDisabled(true);
    } else {
        ui->pushButton_editQuestions_viewClassList->setDisabled(false);
        ui->pushButton_results_viewClassList->setDisabled(false);
        ui->pushButton_questions->setDisabled(false);
    }
}

void Teacherinterface::on_pushButton_editQuestions_viewClassList_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);

    ui->label_status_edit_viewClass->clear();
    ui->label_status_edit_viewClass->clear();

    ui->lineEdit_Question1_edit_viewClass->clear();
    ui->lineEdit_answer1_edit_viewClass->clear();
    ui->lineEdit_A1_edit_viewClass->clear();
    ui->lineEdit_B1_edit_viewClass->clear();
    ui->lineEdit_C1_edit_viewClass->clear();
    ui->lineEdit_D1_edit_viewClass->clear();

    ui->lineEdit_Question2_edit_viewClass->clear();
    ui->lineEdit_answer2_edit_viewClass->clear();
    ui->lineEdit_A2_edit_viewClass->clear();
    ui->lineEdit_B2_edit_viewClass->clear();
    ui->lineEdit_C2_edit_viewClass->clear();
    ui->lineEdit_D2_edit_viewClass->clear();

    ui->lineEdit_Question3_edit_viewClass->clear();
    ui->lineEdit_answer3_edit_viewClass->clear();
    ui->lineEdit_A3_edit_viewClass->clear();
    ui->lineEdit_B3_edit_viewClass->clear();
    ui->lineEdit_C3_edit_viewClass->clear();
    ui->lineEdit_D3_edit_viewClass->clear();

    ui->lineEdit_Question4_edit_viewClass->clear();
    ui->lineEdit_answer4_edit_viewClass->clear();
    ui->lineEdit_A4_edit_viewClass->clear();
    ui->lineEdit_B4_edit_viewClass->clear();
    ui->lineEdit_C4_edit_viewClass->clear();
    ui->lineEdit_D4_edit_viewClass->clear();

    ui->lineEdit_Question5_edit_viewClass->clear();
    ui->lineEdit_answer5_edit_viewClass->clear();
    ui->lineEdit_A5_edit_viewClass->clear();
    ui->lineEdit_B5_edit_viewClass->clear();
    ui->lineEdit_C5_edit_viewClass->clear();
    ui->lineEdit_D5_edit_viewClass->clear();

    ui->lineEdit_Question6_edit_viewClass->clear();
    ui->lineEdit_answer6_edit_viewClass->clear();
    ui->lineEdit_A6_edit_viewClass->clear();
    ui->lineEdit_B6_edit_viewClass->clear();
    ui->lineEdit_C6_edit_viewClass->clear();
    ui->lineEdit_D6_edit_viewClass->clear();

    ui->lineEdit_Question7_edit_viewClass->clear();
    ui->lineEdit_answer7_edit_viewClass->clear();
    ui->lineEdit_A7_edit_viewClass->clear();
    ui->lineEdit_B7_edit_viewClass->clear();
    ui->lineEdit_C7_edit_viewClass->clear();
    ui->lineEdit_D7_edit_viewClass->clear();

    ui->lineEdit_Question8_edit_viewClass->clear();
    ui->lineEdit_answer8_edit_viewClass->clear();
    ui->lineEdit_A8_edit_viewClass->clear();
    ui->lineEdit_B8_edit_viewClass->clear();
    ui->lineEdit_C8_edit_viewClass->clear();
    ui->lineEdit_D8_edit_viewClass->clear();

    ui->lineEdit_Question9_edit_viewClass->clear();
    ui->lineEdit_answer9_edit_viewClass->clear();
    ui->lineEdit_A9_edit_viewClass->clear();
    ui->lineEdit_B9_edit_viewClass->clear();
    ui->lineEdit_C9_edit_viewClass->clear();
    ui->lineEdit_D9_edit_viewClass->clear();

    ui->lineEdit_Question10_edit_viewClass->clear();
    ui->lineEdit_answer10_edit_viewClass->clear();
    ui->lineEdit_A10_edit_viewClass->clear();
    ui->lineEdit_B10_edit_viewClass->clear();
    ui->lineEdit_C10_edit_viewClass->clear();
    ui->lineEdit_D10_edit_viewClass->clear();


    ui->label_edit_viewClass->setText("Edit Subject: " + ui->comboBox_subject_viewClassList->currentText());
    ui->label_editContinue_viewClass->setText("Edit Subject: " + ui->comboBox_subject_viewClassList->currentText());

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=1;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question1_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer1_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A1_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B1_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C1_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D1_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();

    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=2;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question2_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer2_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A2_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B2_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C2_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D2_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=3;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question3_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer3_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A3_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B3_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C3_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D3_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=4;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question4_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer4_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A4_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B4_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C4_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D4_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=5;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question5_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer5_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A5_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B5_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C5_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D5_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=6;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question6_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer6_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A6_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B6_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C6_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D6_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=7;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while(qry->next()) {
            ui->lineEdit_Question7_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer7_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A7_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B7_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C7_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D7_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=8;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while (qry->next()) {
            ui->lineEdit_Question8_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer8_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A8_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B8_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C8_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D8_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=9;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while (qry->next()) {
            ui->lineEdit_Question9_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer9_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A9_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B9_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C9_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D9_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }

    qry->prepare("select * from '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice' where No=10;");

    if(qry->exec()) {
        qDebug() << "Multiple Choice updated:" << qry->lastError();
        while (qry->next()) {
            ui->lineEdit_Question10_edit_viewClass->setText(qry->value("Questions").toString());
            ui->lineEdit_answer10_edit_viewClass->setText(qry->value("Answer").toString());
            ui->lineEdit_A10_edit_viewClass->setText(qry->value("A").toString());
            ui->lineEdit_B10_edit_viewClass->setText(qry->value("B").toString());
            ui->lineEdit_C10_edit_viewClass->setText(qry->value("C").toString());
            ui->lineEdit_D10_edit_viewClass->setText(qry->value("D").toString());
        }
    } else {
        qDebug() << "Multiple Choice error:" << qry->lastError() << qry->lastQuery();
    }
}

void Teacherinterface::on_pushButton_cancel_edit_viewClass_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void Teacherinterface::on_pushButton_next_edit_viewClass_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void Teacherinterface::on_pushButton_previous_editContinue_viewClass_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void Teacherinterface::on_pushButton_update_editContinue_viewClass_clicked()
{
    if(ui->lineEdit_Question1_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer1_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A1_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B1_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C1_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D1_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question2_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer2_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A2_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B2_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C2_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D2_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question3_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer3_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A3_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B3_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C3_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D3_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question4_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer4_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A4_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B4_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C4_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D4_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question5_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer5_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A5_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B5_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C5_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D5_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question6_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer6_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A6_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B6_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C6_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D6_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question7_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer7_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A7_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B7_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C7_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D7_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question8_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer8_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A8_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B8_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C8_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D8_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question9_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer9_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A9_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B9_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C9_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D9_edit_viewClass->text().isEmpty() ||

       ui->lineEdit_Question10_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_answer10_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_A10_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_B10_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_C10_edit_viewClass->text().isEmpty() ||
       ui->lineEdit_D10_edit_viewClass->text().isEmpty()) {
        ui->label_status_edit_viewClass->setText("Required Field is Empty");
        ui->label_status_editContinue_viewClass->setText("Required Field is Empty");
    } else {
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question1_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer1_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A1_edit_viewClass->text()+"', B='"+ui->lineEdit_B1_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C1_edit_viewClass->text()+"', D='"+ui->lineEdit_D1_edit_viewClass->text()+"' where rowid='1';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }

        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question2_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer2_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A2_edit_viewClass->text()+"', B='"+ui->lineEdit_B2_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C2_edit_viewClass->text()+"', D='"+ui->lineEdit_D2_edit_viewClass->text()+"' where rowid='2';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question3_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer3_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A3_edit_viewClass->text()+"', B='"+ui->lineEdit_B3_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C3_edit_viewClass->text()+"', D='"+ui->lineEdit_D3_edit_viewClass->text()+"' where rowid='3';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question4_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer4_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A4_edit_viewClass->text()+"', B='"+ui->lineEdit_B4_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C4_edit_viewClass->text()+"', D='"+ui->lineEdit_D4_edit_viewClass->text()+"' where rowid='4';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question5_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer5_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A5_edit_viewClass->text()+"', B='"+ui->lineEdit_B5_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C5_edit_viewClass->text()+"', D='"+ui->lineEdit_D5_edit_viewClass->text()+"' where rowid='5';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question6_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer6_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A6_edit_viewClass->text()+"', B='"+ui->lineEdit_B6_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C6_edit_viewClass->text()+"', D='"+ui->lineEdit_D6_edit_viewClass->text()+"' where rowid='6';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question7_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer7_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A7_edit_viewClass->text()+"', B='"+ui->lineEdit_B7_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C7_edit_viewClass->text()+"', D='"+ui->lineEdit_D7_edit_viewClass->text()+"' where rowid='7';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question8_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer8_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A8_edit_viewClass->text()+"', B='"+ui->lineEdit_B8_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C8_edit_viewClass->text()+"', D='"+ui->lineEdit_D8_edit_viewClass->text()+"' where rowid='8';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question9_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer9_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A9_edit_viewClass->text()+"', B='"+ui->lineEdit_B9_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C9_edit_viewClass->text()+"', D='"+ui->lineEdit_D9_edit_viewClass->text()+"' where rowid='9';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        qry->prepare("update '" + tempId + "Class" + ui->comboBox_subject_viewClassList->currentText() +"MultipleChoice' "
                     "set Questions='"+ui->lineEdit_Question10_edit_viewClass->text()+"', Answer='"+ui->lineEdit_answer10_edit_viewClass->text()+
                     "', A='"+ui->lineEdit_A10_edit_viewClass->text()+"', B='"+ui->lineEdit_B10_edit_viewClass->text()+
                     "', C='"+ui->lineEdit_C10_edit_viewClass->text()+"', D='"+ui->lineEdit_D10_edit_viewClass->text()+"' where rowid='10';");

        if(qry->exec()) {
            qDebug() << "Class Questions Updated :" << qry->lastError();
        } else {
            qDebug() << "Class Questions Error :" << qry->lastError() << qry->lastQuery();
        }
        ui->label_status_edit_viewClass->setText("Successfully Updated Questions");
        ui->label_status_edit_viewClass->setText("Successfully Updated Questions");

        ui->lineEdit_Question1_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer1_edit_viewClass->setDisabled(true);
        ui->lineEdit_A1_edit_viewClass->setDisabled(true);
        ui->lineEdit_B1_edit_viewClass->setDisabled(true);
        ui->lineEdit_C1_edit_viewClass->setDisabled(true);
        ui->lineEdit_D1_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question2_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer2_edit_viewClass->setDisabled(true);
        ui->lineEdit_A2_edit_viewClass->setDisabled(true);
        ui->lineEdit_B2_edit_viewClass->setDisabled(true);
        ui->lineEdit_C2_edit_viewClass->setDisabled(true);
        ui->lineEdit_D2_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question3_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer3_edit_viewClass->setDisabled(true);
        ui->lineEdit_A3_edit_viewClass->setDisabled(true);
        ui->lineEdit_B3_edit_viewClass->setDisabled(true);
        ui->lineEdit_C3_edit_viewClass->setDisabled(true);
        ui->lineEdit_D3_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question4_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer4_edit_viewClass->setDisabled(true);
        ui->lineEdit_A4_edit_viewClass->setDisabled(true);
        ui->lineEdit_B4_edit_viewClass->setDisabled(true);
        ui->lineEdit_C4_edit_viewClass->setDisabled(true);
        ui->lineEdit_D4_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question5_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer5_edit_viewClass->setDisabled(true);
        ui->lineEdit_A5_edit_viewClass->setDisabled(true);
        ui->lineEdit_B5_edit_viewClass->setDisabled(true);
        ui->lineEdit_C5_edit_viewClass->setDisabled(true);
        ui->lineEdit_D5_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question6_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer6_edit_viewClass->setDisabled(true);
        ui->lineEdit_A6_edit_viewClass->setDisabled(true);
        ui->lineEdit_B6_edit_viewClass->setDisabled(true);
        ui->lineEdit_C6_edit_viewClass->setDisabled(true);
        ui->lineEdit_D6_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question7_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer7_edit_viewClass->setDisabled(true);
        ui->lineEdit_A7_edit_viewClass->setDisabled(true);
        ui->lineEdit_B7_edit_viewClass->setDisabled(true);
        ui->lineEdit_C7_edit_viewClass->setDisabled(true);
        ui->lineEdit_D7_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question8_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer8_edit_viewClass->setDisabled(true);
        ui->lineEdit_A8_edit_viewClass->setDisabled(true);
        ui->lineEdit_B8_edit_viewClass->setDisabled(true);
        ui->lineEdit_C8_edit_viewClass->setDisabled(true);
        ui->lineEdit_D8_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question9_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer9_edit_viewClass->setDisabled(true);
        ui->lineEdit_A9_edit_viewClass->setDisabled(true);
        ui->lineEdit_B9_edit_viewClass->setDisabled(true);
        ui->lineEdit_C9_edit_viewClass->setDisabled(true);
        ui->lineEdit_D9_edit_viewClass->setDisabled(true);

        ui->lineEdit_Question10_edit_viewClass->setDisabled(true);
        ui->lineEdit_answer10_edit_viewClass->setDisabled(true);
        ui->lineEdit_A10_edit_viewClass->setDisabled(true);
        ui->lineEdit_B10_edit_viewClass->setDisabled(true);
        ui->lineEdit_C10_edit_viewClass->setDisabled(true);
        ui->lineEdit_D10_edit_viewClass->setDisabled(true);

        ui->pushButton_cancel_editCountinue_viewClass->setDisabled(true);
        ui->pushButton_update_editContinue_viewClass->setDisabled(true);
        delay(1500);
        ui->label_status_edit_viewClass->clear();
        ui->label_status_edit_viewClass->clear();

        ui->lineEdit_Question1_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer1_edit_viewClass->setDisabled(false);
        ui->lineEdit_A1_edit_viewClass->setDisabled(false);
        ui->lineEdit_B1_edit_viewClass->setDisabled(false);
        ui->lineEdit_C1_edit_viewClass->setDisabled(false);
        ui->lineEdit_D1_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question2_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer2_edit_viewClass->setDisabled(false);
        ui->lineEdit_A2_edit_viewClass->setDisabled(false);
        ui->lineEdit_B2_edit_viewClass->setDisabled(false);
        ui->lineEdit_C2_edit_viewClass->setDisabled(false);
        ui->lineEdit_D2_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question3_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer3_edit_viewClass->setDisabled(false);
        ui->lineEdit_A3_edit_viewClass->setDisabled(false);
        ui->lineEdit_B3_edit_viewClass->setDisabled(false);
        ui->lineEdit_C3_edit_viewClass->setDisabled(false);
        ui->lineEdit_D3_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question4_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer4_edit_viewClass->setDisabled(false);
        ui->lineEdit_A4_edit_viewClass->setDisabled(false);
        ui->lineEdit_B4_edit_viewClass->setDisabled(false);
        ui->lineEdit_C4_edit_viewClass->setDisabled(false);
        ui->lineEdit_D4_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question5_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer5_edit_viewClass->setDisabled(false);
        ui->lineEdit_A5_edit_viewClass->setDisabled(false);
        ui->lineEdit_B5_edit_viewClass->setDisabled(false);
        ui->lineEdit_C5_edit_viewClass->setDisabled(false);
        ui->lineEdit_D5_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question6_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer6_edit_viewClass->setDisabled(false);
        ui->lineEdit_A6_edit_viewClass->setDisabled(false);
        ui->lineEdit_B6_edit_viewClass->setDisabled(false);
        ui->lineEdit_C6_edit_viewClass->setDisabled(false);
        ui->lineEdit_D6_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question7_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer7_edit_viewClass->setDisabled(false);
        ui->lineEdit_A7_edit_viewClass->setDisabled(false);
        ui->lineEdit_B7_edit_viewClass->setDisabled(false);
        ui->lineEdit_C7_edit_viewClass->setDisabled(false);
        ui->lineEdit_D7_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question8_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer8_edit_viewClass->setDisabled(false);
        ui->lineEdit_A8_edit_viewClass->setDisabled(false);
        ui->lineEdit_B8_edit_viewClass->setDisabled(false);
        ui->lineEdit_C8_edit_viewClass->setDisabled(false);
        ui->lineEdit_D8_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question9_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer9_edit_viewClass->setDisabled(false);
        ui->lineEdit_A9_edit_viewClass->setDisabled(false);
        ui->lineEdit_B9_edit_viewClass->setDisabled(false);
        ui->lineEdit_C9_edit_viewClass->setDisabled(false);
        ui->lineEdit_D9_edit_viewClass->setDisabled(false);

        ui->lineEdit_Question10_edit_viewClass->setDisabled(false);
        ui->lineEdit_answer10_edit_viewClass->setDisabled(false);
        ui->lineEdit_A10_edit_viewClass->setDisabled(false);
        ui->lineEdit_B10_edit_viewClass->setDisabled(false);
        ui->lineEdit_C10_edit_viewClass->setDisabled(false);
        ui->lineEdit_D10_edit_viewClass->setDisabled(false);

        ui->pushButton_cancel_editCountinue_viewClass->setDisabled(false);
        ui->pushButton_update_editContinue_viewClass->setDisabled(false);
        on_actionView_Class_List_triggered();
    }
}

void Teacherinterface::on_pushButton_cancel_editCountinue_viewClass_clicked()
{
    on_pushButton_cancel_edit_viewClass_clicked();
}

void Teacherinterface::on_pushButton_results_viewClassList_clicked()
{
    ui->tableView_viewClassList->show();
    QSqlQuery query;
    query.prepare("select * from '"+tempId +"Class"+ui->comboBox_subject_viewClassList->currentText()+"Result';");
    if(query.exec()) {
        qDebug() << "View Class Result Updated: " << query.lastError();
        model->setQuery(query);
        ui->tableView_viewClassList->setModel(model);
    } else {
        qDebug() << "View Class Result Error: " << query.lastError() << query.lastQuery();
    }
}

void Teacherinterface::on_pushButton_questions_clicked()
{
    ui->tableView_viewClassList->show();
//    QSqlQuery query;
//    query.prepare("select * from '"+tempId +"Class"+ui->comboBox_subject_viewClassList->currentText()+"MultipleChoice';");
//    if(query.exec()) {
//        qDebug() << "View Class Result Updated: " << query.lastError();
//        model->setQuery(query);
//        ui->tableView_viewClassList->setModel(model);
//    } else {
//        qDebug() << "View Class Result Error: " << query.lastError() << query.lastQuery();
//    }

    tableModel->setTable(tempId +"Class" + ui->comboBox_subject_viewClassList->currentText() + "MultipleChoice");
    tableModel->select();
    ui->tableView_viewClassList->setModel(tableModel);
    ui->tableView_viewClassList->resizeColumnsToContents();
    tableModel->submitAll();
}
