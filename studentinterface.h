#ifndef STUDENTINTERFACE_H
#define STUDENTINTERFACE_H

#include "logindialog.h"

namespace Ui {
class StudentInterface;
}

class StudentInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentInterface(const QString &TempID, const QString &TempPassword, QSqlDatabase &db);
    ~StudentInterface();

private slots:
    void on_actionExit_triggered();
    void on_actionLogout_triggered();
    void on_actionAbout_triggered();
    void on_actionClass_triggered();
    void on_actionWelcome_triggered();
    void on_actionMy_Info_triggered();
    void on_pushButton_next_quiz_clicked();
    void on_comboBox_teacher_currentIndexChanged(const QString &arg1);
    void on_pushButton_startEnd_quiz_clicked();
    void updateLCD();
    void on_comboBox_subject_currentIndexChanged(const QString &arg1);
    void on_pushButton_previous_quiz_2_clicked();
    void on_pushButton_end_quiz_2_clicked();

private:
    Ui::StudentInterface *ui;
    QString tempID;
    QString tempPassword;
    QString tempTeacherID;
    QString tempTeacherClasslist;
    QString tempSubject;
    QString A1;
    QString A2;
    QString A3;
    QString A4;
    QString A5;
    QString A6;
    QString A7;
    QString A8;
    QString A9;
    QString A10;
    QSqlQuery *qry;
    QTimer *timer;
    QTime time;
    QTime timeEnd;
    void delay(int millisecondsToWait);

};

#endif // STUDENTINTERFACE_H
