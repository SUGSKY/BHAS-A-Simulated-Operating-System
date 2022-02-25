#include<QDebug>
#include<QHBoxLayout>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QPalette>
#include<QPixmap>
#include<QScreen>
#include<QWidget>
#include"loadingpage.h"
#include "loginpage.h"
#include "ui_loginpage.h"

//Global Variables;
const QString username = "Admin";
const QString password = "CSC3002-2020";
const QString userIDName = "User3002";

loginPage::loginPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginPage)
{
    ui->setupUi(this);
    //setup
    QFont loginFont, userIdFont;
    QPixmap backgroundImagePixmap(":/img/background.jpg");
    QPixmap loginBoxImage(":/img/loginBox.png");
    QPixmap photoIcon(":/img/loginPhoto.png");
    QPalette backgroundImagePalette = palette();
    backgroundImagePalette.setBrush(QPalette::Background, backgroundImagePixmap.scaled(qApp->screens()[0]->size(), Qt::IgnoreAspectRatio));
    QPalette loginBoxColor = palette();
    this->setPalette(backgroundImagePalette);

    loadingPage *loadingpage = new loadingPage(this);
    loadingpage->showFullScreen();
    this->hide();
    connect(loadingpage, &loadingPage::forcedClosed, this, &loginPage::close);

    //widgets
    photo = new QLabel(this);
    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    login = new QPushButton(this);
    QLabel *userIDText = new QLabel(this);
    QLabel *loginText = new QLabel(this);
    QLabel *usernameText = new QLabel(this);
    QLabel *passwordText = new QLabel(this);

    //widgets initialization
    photo->setPixmap(photoIcon.scaled(360,360,Qt::KeepAspectRatio));
    photo->setFixedSize(360, 360);
    usernameInput->setMaxLength(15);
    usernameInput->setFixedSize(250, 35);
    passwordInput->setMaxLength(30);
    passwordInput->setFixedSize(250, 35);
    passwordInput->setEchoMode(QLineEdit::Password);
    login->setText("Login");
    login->setFixedSize(90, 35);
    login->setAutoDefault(1);
    userIdFont.setBold(1);
    userIdFont.setPointSize(10);
    userIDText->setText(userIDName);
    userIDText->setAlignment(Qt::AlignCenter);
    userIDText->setFont(userIdFont);
    userIDText->setStyleSheet("QLabel{color:white}");
    userIDText->setFixedSize(200, 70);
    loginFont.setBold(1);
    loginFont.setPointSize(20);
    loginText->setText("LOGIN");
    loginText->setFont(loginFont);
    loginText->setStyleSheet("QLabel{color:white}");
    usernameText->setText("Username:");
    usernameText->setFixedSize(100, 35);
    usernameText->setStyleSheet("QLabel{font-weight: bold; color:white}");
    passwordText->setText("Password:");
    passwordText->setFixedSize(100, 35);
    passwordText->setStyleSheet("QLabel{font-weight: bold; color:white}");

    //layout
    QWidget *userIDWidget = new QWidget;
    QVBoxLayout *userIDLayout = new QVBoxLayout(userIDWidget);
    userIDWidget->setFixedSize(450,450);
    userIDLayout->addWidget(photo, 0, Qt::AlignTop | Qt::AlignHCenter);
    userIDLayout->addSpacing(20);
    userIDLayout->addWidget(userIDText, 0, Qt::AlignTop | Qt::AlignHCenter);


    QWidget *usernameWidget = new QWidget;
    QHBoxLayout *usernameLayout = new QHBoxLayout(usernameWidget);
    usernameLayout->addWidget(usernameText, 0, Qt::AlignLeft);
    usernameLayout->addWidget(usernameInput, 0, Qt::AlignRight);

    QWidget *passwordWidget = new QWidget;
    QHBoxLayout *passwordLayout = new QHBoxLayout(passwordWidget);
    passwordLayout->addWidget(passwordText, 0, Qt::AlignLeft);
    passwordLayout->addWidget(passwordInput, 0, Qt::AlignRight);

    QWidget *loginBoxWidget = new QWidget;
    QVBoxLayout *loginBoxLayout = new QVBoxLayout(loginBoxWidget);
    loginBoxWidget->setFixedSize(450,600);
    loginBoxWidget->setAutoFillBackground(1);
    loginBoxColor.setBrush(QPalette::Background, loginBoxImage.scaled(loginBoxWidget->size(), Qt::IgnoreAspectRatio));
    loginBoxWidget->setPalette(loginBoxColor);
    loginBoxLayout->addSpacing(80);
    loginBoxLayout->addWidget(loginText, 0, Qt::AlignCenter);
    loginBoxLayout->addSpacing(50);
    loginBoxLayout->addWidget(usernameWidget, 0, Qt::AlignTop | Qt::AlignHCenter);
    loginBoxLayout->addWidget(passwordWidget, 0, Qt::AlignTop | Qt::AlignHCenter);
    loginBoxLayout->addWidget(login, 0, Qt::AlignTop | Qt::AlignHCenter);
    loginBoxLayout->addSpacing(100);

    ui->mainLayout->addWidget(userIDWidget);
    ui->mainLayout->addWidget(loginBoxWidget, 0, Qt::AlignVCenter);


    //connection
    connect(login, &QPushButton::clicked, this, &loginPage::Attempt);
}

loginPage::~loginPage()
{
    delete ui;
}

void loginPage::Attempt(){
    QString currUsername = loginPage::usernameInput->text();
    QString currPassword = loginPage::passwordInput->text();
    if(currUsername == username && currPassword == password){
        parentWidget()->show();
        delete this;
    }else{
        QMessageBox::warning(this, "Login Failed", "You have inputted wrong username/password. Try again!");
        loginPage::usernameInput->setText("");
        loginPage::passwordInput->setText("");
    }
}

void loginPage::closeEvent(QCloseEvent *event){
    emit forcedClosed();
}
