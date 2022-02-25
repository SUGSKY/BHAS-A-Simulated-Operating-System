#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include<QMainWindow>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QString>

namespace Ui {
class loginPage;
}

class loginPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginPage(QWidget *parent = nullptr);
    ~loginPage();

signals:
    void forcedClosed();


private:
    Ui::loginPage *ui;

    QLabel* LOGO;
    QLabel* photo;
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QPushButton* login;

    void Attempt();
    void closeEvent(QCloseEvent *event);
};

#endif // LOGINPAGE_H
