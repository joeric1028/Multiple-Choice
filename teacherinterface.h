#ifndef TEACHERINTERFACE_H
#define TEACHERINTERFACE_H

#include "logindialog.h"

namespace Ui {
class Teacherinterface;
}

class Teacherinterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit Teacherinterface(const QString &TempID, const QString &TempPassword, QSqlDatabase &db);
    ~Teacherinterface();

private slots:

    void on_actionExit_triggered();
    void on_actionLogout_triggered();
    void on_actionCreate_Teacher_triggered();
    void on_actionWelcome_triggered();
    void on_actionAbout_triggered();
    void on_actionCreate_Student_triggered();
    void on_actionMy_Info_triggered();
    void on_pushButton_edit_information_myInfo_clicked();
    void on_pushButton_update_edit_myInfo_clicked();
    void on_pushButton_cancel_edit_myInfo_clicked();
    void on_pushButton_cancel_createTeacher_clicked();
    void on_pushButton_createAccount_createTeacher_clicked();
    void on_pushButton_createAccount_createStudent_clicked();
    void on_pushButton_cancel_createStudent_clicked();
    void on_actionDelete_triggered();
    void on_actionView_Student_Profile_triggered();
    void on_pushButton_delete_clicked();
    void on_lineEdit_filterBar_delete_textChanged(const QString &arg1);
    void on_comboBox_selectStudentID_viewStudentProfile_currentIndexChanged(const QString &arg1);
    void on_pushButton_edit_information_viewStudentProfile_clicked();
    void on_pushButton_cancel_edit_viewStudentProfile_clicked();
    void on_pushButton_update_edit_viewStudentProfile_clicked();
    void on_actionCreate_Class_triggered();
    void on_actionView_Class_List_triggered();
    void on_pushButton_create_createClass_clicked();
    void on_pushButton_cancel_createClass_clicked();
    void on_comboBox_subject_viewClassList_currentIndexChanged(const QString &arg1);
    void on_pushButton_editQuestions_viewClassList_clicked();
    void on_pushButton_cancel_edit_viewClass_clicked();
    void on_pushButton_next_edit_viewClass_clicked();
    void on_pushButton_previous_editContinue_viewClass_clicked();
    void on_pushButton_update_editContinue_viewClass_clicked();
    void on_pushButton_cancel_editCountinue_viewClass_clicked();
    void on_pushButton_results_viewClassList_clicked();
    void on_pushButton_questions_clicked();

private:
    Ui::Teacherinterface *ui;
    QString tempId;
    QString tempPassword;
    QString tempSubject;
    QString tempdeleteId;
    QSqlQuery *qry;
    QSqlQueryModel *model;
    QSqlTableModel *tableModel;
    void delay(int millisecondsToWait);

};

#endif // TEACHERINTERFACE_H
