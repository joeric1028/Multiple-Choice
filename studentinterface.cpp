#include "studentinterface.h"
#include "ui_studentinterface.h"

StudentInterface::StudentInterface(const QString &TempID, const QString &TempPassword, QSqlDatabase &db) : ui(new Ui::StudentInterface)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    tempID = TempID;
    tempPassword = TempPassword;
    qDebug() << "Received Data! From LoginDialog to StudentInterface:" << tempID << tempPassword;
    db = QSqlDatabase::database();
    qry = new QSqlQuery(db);
    qry->prepare("Select * from Student where ID=:tempId and Password=:tempPassword");
    qry->bindValue(":tempId", tempID);
    qry->bindValue(":tempPassword", tempPassword);
    qry->exec();
    while(qry->next()) {
        ui->label_studentGreetings_welcome->setText("Welcome, " + qry->value("FirstName").toString() + " " + qry->value("MiddleName").toString() + " " + qry->value("LastName").toString() + "!");
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateLCD()));
    timeEnd.setHMS(0,0,0);
}

StudentInterface::~StudentInterface()
{
    delete ui;
}

void StudentInterface::on_actionExit_triggered()
{
    exit(0);
}

void StudentInterface::on_actionLogout_triggered()
{
    this->close();
    LoginDialog std;
    std.exec();
}

void StudentInterface::on_actionAbout_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StudentInterface::on_actionClass_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);

    ui->comboBox_teacher->clear();
    ui->comboBox_subject->setDisabled(true);
    ui->pushButton_next_quiz->setDisabled(true);
    ui->pushButton_startEnd_quiz->setDisabled(true);
    ui->groupBox_questions->hide();
    ui->comboBox_teacher->addItem("Select Teacher");

    qry->prepare("Select LastName from Teacher;");

    if(qry->exec()) {
        while(qry->next()) {
            ui->comboBox_teacher->addItem(qry->value("LastName").toString());
        }
        qDebug() << "Retriveing Teacher list to combobox teacher found " << qry->lastError();
    } else {
        qDebug() << "Retriveing Teacher list to combobox teacher Error " << qry->lastError() << qry->lastQuery();
    }
}

void StudentInterface::on_actionWelcome_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StudentInterface::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void StudentInterface::on_pushButton_next_quiz_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void StudentInterface::on_actionMy_Info_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
    qry->prepare("select * from Student where ID=:tempId;");
    qry->bindValue(":tempId", tempID);

    if (qry->exec()) {
        qDebug() << "View Student Profile Updated: " << qry->lastError();
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

void StudentInterface::on_comboBox_teacher_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Select Teacher") {
        ui->comboBox_subject->clear();
        ui->comboBox_subject->addItem("Select Subject");
        ui->comboBox_subject->setDisabled(true);
        ui->pushButton_startEnd_quiz->setDisabled(true);
    } else {
        ui->comboBox_subject->clear();
        ui->comboBox_subject->addItem("Select Subject");
        ui->comboBox_subject->setDisabled(false);
        qry->prepare("Select ID from Teacher where LastName='" + arg1 +"';");
        qDebug() << arg1;
        if(qry->exec()) {
            while(qry->next()) {
                tempTeacherID = qry->value("ID").toString();
                qDebug() << "Received Teacher ID" << tempTeacherID;
                qry->prepare("Select Subject, TimeLimit from '" + tempTeacherID + "ClassList';");
                if(qry->exec()) {
                    qDebug() << "Retriving Teacher " << tempTeacherID << " Class List found " << qry->lastError();
                    while(qry->next()) {
                        ui->comboBox_subject->addItem(qry->value("Subject").toString());
                    }
                } else {
                    qDebug() << "Retriving Teacher " << tempTeacherID << " Class List failed " << qry->lastError() << qry->lastQuery();
                }
            }
            qDebug() << "Retriving Class list found " << qry->lastError() << qry->lastQuery();
        } else {
            qDebug() << "Retriving Class list Error " << qry->lastError() << qry->lastQuery();
        }
    }
}

void StudentInterface::on_pushButton_startEnd_quiz_clicked()
{
    qry->prepare("select * from '" + tempTeacherID + "Class" + tempSubject + "Result' where StudentID='" + tempID + "';");

    if (qry->exec()) {
        qDebug() << "Student Checked" << qry->lastError();
        int count = 0;
        while (qry->next()) {
            count++;
        }
        if (count < 1) {
            ui->toolBar->hide();
            ui->pushButton_startEnd_quiz->setDisabled(true);
            ui->pushButton_next_quiz->setDisabled(false);
            ui->comboBox_subject->setDisabled(true);
            ui->comboBox_teacher->setDisabled(true);
            timer->start(1000);
            ui->groupBox_questions->show();
        }
        if (count == 1) {
            QMessageBox mess;
            QIcon icon;
            QPixmap pic;
            pic.load(":/logo/quizcreator-w250.png");
            icon.addPixmap(pic);
            mess.setWindowTitle("Quiz Creator 2016 v1.2");
            mess.setWindowIcon(pic);
            mess.setText("Already taken the Quiz.");
            mess.setModal(true);
            mess.exec();
        }
    } else {
        qDebug() << "Student Check error" << qry->lastError() << qry->lastQuery();
    }
}

void StudentInterface::updateLCD()
{
    time = time.addSecs(-1);
    QString timeText = time.toString("hh:mm:ss");
    QString timeend = timeEnd.toString("hh:mm:ss");
    ui->lcdNumber->display(timeText);
    ui->lcdNumber_2->display(timeText);
    qDebug() << "Time left" << time;
    qDebug() << "Time end" << timeEnd;
    if(timeText == timeend)
    {
        timer->stop();
        on_pushButton_end_quiz_2_clicked();
    }
}

void StudentInterface::on_comboBox_subject_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Select Subject") {
        ui->lcdNumber->display(0);
        ui->lcdNumber_2->display(0);
        ui->pushButton_startEnd_quiz->setDisabled(true);
    }
    qry->prepare("Select TimeLimit from '" + tempTeacherID + "ClassList' where Subject='" + arg1 + "';");
    if(qry->exec()) {
        qDebug() << "Retriving Teacher Timelimit :" << qry->lastError();
        while(qry->next()) {
            time.setHMS(0, qry->value("TimeLimit").toInt(), 0);
            QString timeText = time.toString("hh:mm:ss");
            ui->lcdNumber->display(timeText);
            ui->lcdNumber_2->display(timeText);
        }
    } else {
        qDebug() << "Retriving Teacher " << tempTeacherID << " Class List failed " << qry->lastError() << qry->lastQuery();
    }
    tempSubject = arg1;
    qDebug() << "Receiving Teacher Subject " << tempSubject;
    qry->prepare("select * from '" + tempTeacherID + "Class" + tempSubject + "MultipleChoice' order by random();");
    if(qry->exec()) {
        ui->pushButton_startEnd_quiz->setDisabled(false);
        qDebug() << "Receiving Teacher Questionaire completed " << qry->lastError();
        qry->next();
        ui->Q1->setText(qry->value("Questions").toString());
        ui->A1->setText(qry->value("D").toString());
        ui->B1->setText(qry->value("A").toString());
        ui->C1->setText(qry->value("C").toString());
        ui->D1->setText(qry->value("B").toString());
        A1 = qry->value("Answer").toString();

        qry->next();
        ui->Q2->setText(qry->value("Questions").toString());
        ui->A2->setText(qry->value("A").toString());
        ui->B2->setText(qry->value("B").toString());
        ui->C2->setText(qry->value("C").toString());
        ui->D2->setText(qry->value("D").toString());
        A2 = qry->value("Answer").toString();

        qry->next();
        ui->Q3->setText(qry->value("Questions").toString());
        ui->A3->setText(qry->value("D").toString());
        ui->B3->setText(qry->value("A").toString());
        ui->C3->setText(qry->value("C").toString());
        ui->D3->setText(qry->value("B").toString());
        A3 = qry->value("Answer").toString();

        qry->next();
        ui->Q4->setText(qry->value("Questions").toString());
        ui->A4->setText(qry->value("C").toString());
        ui->B4->setText(qry->value("B").toString());
        ui->C4->setText(qry->value("A").toString());
        ui->D4->setText(qry->value("D").toString());
        A4 = qry->value("Answer").toString();

        qry->next();
        ui->Q5->setText(qry->value("Questions").toString());
        ui->A5->setText(qry->value("D").toString());
        ui->B5->setText(qry->value("A").toString());
        ui->C5->setText(qry->value("C").toString());
        ui->D5->setText(qry->value("B").toString());
        A5 = qry->value("Answer").toString();

        qry->next();
        ui->Q6->setText(qry->value("Questions").toString());
        ui->A6->setText(qry->value("A").toString());
        ui->B6->setText(qry->value("C").toString());
        ui->C6->setText(qry->value("B").toString());
        ui->D6->setText(qry->value("D").toString());
        A6 = qry->value("Answer").toString();

        qry->next();
        ui->Q7->setText(qry->value("Questions").toString());
        ui->A7->setText(qry->value("A").toString());
        ui->B7->setText(qry->value("D").toString());
        ui->C7->setText(qry->value("B").toString());
        ui->D7->setText(qry->value("C").toString());
        A7 = qry->value("Answer").toString();

        qry->next();
        ui->Q8->setText(qry->value("Questions").toString());
        ui->A8->setText(qry->value("D").toString());
        ui->B8->setText(qry->value("A").toString());
        ui->C8->setText(qry->value("C").toString());
        ui->D8->setText(qry->value("B").toString());
        A8 = qry->value("Answer").toString();

        qry->next();
        ui->Q9->setText(qry->value("Questions").toString());
        ui->A9->setText(qry->value("A").toString());
        ui->B9->setText(qry->value("C").toString());
        ui->C9->setText(qry->value("D").toString());
        ui->D9->setText(qry->value("B").toString());
        A9 = qry->value("Answer").toString();

        qry->next();
        ui->Q10->setText(qry->value("Questions").toString());
        ui->A10->setText(qry->value("D").toString());
        ui->B10->setText(qry->value("A").toString());
        ui->C10->setText(qry->value("C").toString());
        ui->D10->setText(qry->value("B").toString());
        A10 = qry->value("Answer").toString();

    } else {
        qDebug() << "Receiving Teacher Questionaire failed " << qry->lastError() << qry->lastQuery();
    }
}

void StudentInterface::on_pushButton_previous_quiz_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void StudentInterface::on_pushButton_end_quiz_2_clicked()
{
    int score = 0;
    timer->stop();
    if(ui->buttonGroup->checkedButton() != 0)
    {
        if(ui->buttonGroup->checkedButton()->text() == A1)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_2->checkedButton() != 0)
    {
        if(ui->buttonGroup_2->checkedButton()->text() == A2)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_3->checkedButton() != 0)
    {
        if(ui->buttonGroup_3->checkedButton()->text() == A3)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_4->checkedButton() != 0)
    {
        if(ui->buttonGroup_4->checkedButton()->text() == A4)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_5->checkedButton() != 0)
    {
        if(ui->buttonGroup_5->checkedButton())
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_6->checkedButton() != 0)
    {
        if(ui->buttonGroup_6->checkedButton()->text() == A6)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_7->checkedButton() != 0)
    {
        if(ui->buttonGroup_7->checkedButton()->text() == A7)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_8->checkedButton() != 0)
    {
        if(ui->buttonGroup_8->checkedButton()->text() == A8)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_9->checkedButton() != 0)
    {
        if(ui->buttonGroup_9->checkedButton()->text() == A9)
        {
            score = score + 1;
        }
    }
    if(ui->buttonGroup_10->checkedButton() != 0)
    {
        if(ui->buttonGroup_10->checkedButton()->text() == A10)
        {
            score = score + 1;
        }
    }
    qry->prepare("insert into '" + tempTeacherID + "Class" + tempSubject + "Result' "
                 "(Score, StudentID) "
                 "values('" + QString::number(score) + "', '" + tempID + "')");
    if(qry->exec()) {
        qDebug() << "Student Score inserted" << score << qry->lastError();
    }

    QMessageBox message;
    QIcon ico;
    ico.addFile(":/logo/quizcreator-w250.png");
    message.setWindowIcon(ico);
    message.setWindowTitle("Quiz Creator 2016 v1.2");
    message.setText("Your Score is " + QString::number(score) + "!");
    message.setModal(true);
    message.exec();

    on_actionClass_triggered();
    ui->comboBox_teacher->setDisabled(false);
    ui->toolBar->show();
}
