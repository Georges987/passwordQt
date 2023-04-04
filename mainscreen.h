#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QtWidgets>
#include <QtSql>
#include <QRegExpValidator>



QT_BEGIN_NAMESPACE
namespace Ui { class MainScreen; }
QT_END_NAMESPACE

class MainScreen : public QWidget
{
    Q_OBJECT

public:
    MainScreen(QWidget *parent = nullptr);
    QString force(QString val);
    bool createConnection();
    QValidator::State check_password(QString value);
    ~MainScreen();

private slots:
    void on_quit_btn_clicked();

    void on_checkBtn_clicked();

    void on_access_database_btn_clicked();

    void on_testing_clicked();

    void on_reset_clicked();

private:
    Ui::MainScreen *ui;
};
#endif // MAINSCREEN_H
