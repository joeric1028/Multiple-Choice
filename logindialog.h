#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>
#include <QSplashScreen>
#include <QMainWindow>
#include <QSqlDriver>

#include "teacherinterface.h"
#include "studentinterface.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void ifRadioButton();
    void on_pushButton_login_clicked();

private:
    Ui::LoginDialog *ui;
    QSqlDatabase db;
    QSqlQuery *qry;
    void delay(int millisecondsToWait);
    void initializeDataBase();
};

#endif // LOGINDIALOG_H
