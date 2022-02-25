#include<QDateTime>
#include<QDebug>
#include<QIcon>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include<QTimer>
#include<QVector>
#include "Calc.h"
#include "ui_Calc.h"

//Global Variables (global variables for this app only)
const QString program_name = "Calculator";             //Change "Desktop" to your app name
//start below

double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(11);
    currAppUsage.append(8);
    currAppUsage.append(6);
    appUsage[program_name] = currAppUsage;

    ui->Display->setText(QString::number(calcVal));

//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    //start below

//Widgets (initialization for widget)
    //start below
    appFeatures.append(new QAction("Close", this));                 //add features (please modify)

    QTimer *updater = new QTimer(this);

    QPushButton *numButtons[10];
    for(int i = 0; i < 10; ++i){
        QString butName = "Button" + QString::number(i);

        numButtons[i] = Calculator::findChild<QPushButton *>(butName);

        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }

    connect(ui->Add, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Equals, SIGNAL(released()), this,
            SLOT(EqualButtonPressed()));
    connect(ui->ChangeSign, SIGNAL(released()), this,
            SLOT(ChangeNumberSign()));

//Widgets setup (set the widget properties)
    //start below
    appFeatures[0]->setStatusTip("Close application");              //set explaination for feature 0 (please modify)

    QMenuBar *dateTime = new QMenuBar(ui->menubar);
    dateAction = new QAction(QDateTime::currentDateTime().date().toString(), this);
    timeAction = new QAction(QDateTime::currentDateTime().time().toString(), this);
    dateTime->addAction(dateAction);
    dateTime->addAction(timeAction);
    ui->menubar->setCornerWidget(dateTime, Qt::TopRightCorner);

    app_list = menuBar()->addMenu("");
    app_list->setIcon(applist_icon);
    appname = openedApps.keys();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    QMenu *app_name = menuBar()->addMenu(program_name);
    //here add app features (e.g. copy, close, etc)
    app_name->addAction(appFeatures[0]);                            //add feature 0 to layout (please modify)



//Layouts (set the display layout)


//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Calculator::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &Calculator::close_app);
    connect(app_list, &QMenu::aboutToShow, this, &Calculator::appListUpdate);
    connect(updater, &QTimer::timeout, this, &Calculator::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    //start below


}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Calculator::switchApps);
    }
}

void Calculator::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void Calculator::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void Calculator::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    close();
}

//add function below

void Calculator::NumPressed(){

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get number on button
    QString butVal = button->text();

    // Get the value in the display
    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){

        // calcVal = butVal.toDouble();
        ui->Display->setText(butVal);

    } else {
        // Put the new number to the right of whats there
        QString newVal = displayVal + butVal;

        // Double version of number
        double dblNewVal = newVal.toDouble();

        // calcVal = newVal.toDouble();

        // Set value in display and allow up to 16
        // digits before using exponents
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));

    }
}

void Calculator::MathButtonPressed(){

    // Cancel out previous math button clicks
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    // Store current value in Display
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get math symbol on the button
    QString butVal = button->text();

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
    } else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    // Clear display
    ui->Display->setText("");

}

void Calculator::EqualButtonPressed(){

    // Holds new calculation
    double solution = 0.0;

    // Get value in display
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    // Make sure a math button was pressed
    if(addTrigger || subTrigger || multTrigger || divTrigger ){
        if(addTrigger){
            solution = calcVal + dblDisplayVal;
        } else if(subTrigger){
            solution = calcVal - dblDisplayVal;
        } else if(multTrigger){
            solution = calcVal * dblDisplayVal;
        } else {
            solution = calcVal / dblDisplayVal;
        }
    }

    // Put solution in display
    ui->Display->setText(QString::number(solution));

}

void Calculator::ChangeNumberSign(){

    // Get the value in the display
    QString displayVal = ui->Display->text();

    // Regular expression checks if it is a number
    // plus sign
    QRegExp reg("[-+]?[0-9.]*");

    // If it is a number change the sign
    if(reg.exactMatch(displayVal)){
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;

        // Put solution in display
        ui->Display->setText(QString::number(dblDisplayValSign));
    }

}

void Calculator::on_Clear_clicked(){
    {
        ui->Display->setText("0");
    }
}






