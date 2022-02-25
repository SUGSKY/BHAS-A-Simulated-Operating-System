#include<QDateTime>                     //new
#include<QDebug>
#include<QIcon>
#include<QPalette>
#include<QPixmap>
#include<QScreen>
#include<QSize>
#include<QTimer>                        //new
#include<QVector>
#include "MyApp.h"
#include "ui_MyApp.h"

//Global Variables (global variables for this app only)
const QString program_name = "Desktop";             //Change "Desktop" to your app name
QList<QString> appname;
QMap<QString, QWidget*> openedApps;
QMap<QString, QVector<int>> appUsage;
const int app_count = 8;
//start here


MyApp::MyApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyApp)
{
    ui->setupUi(this);
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(23);
    currAppUsage.append(18);
    currAppUsage.append(17);
    appUsage[program_name] = currAppUsage;


//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    QIcon notepad_icon(":/img/notepad.png");
    QIcon calendar_icon(":/img/Calendar.png");
    QIcon calc_icon(":/img/Calc.png");
    QIcon taskM_icon(":/img/taskManager.png");
    QIcon GPAPlan_icon(":/img/GPAPlanner.png");
    QIcon kernel_icon(":/img/kernel.png");
    QIcon clock_icon(":/img/clock.png");
    QIcon windowApp_icon(":/img/LOGO.ico");
    QPixmap wallpaperPixmap(":/img/Wallpaper.jpg");
    QPalette wallpaperPalette = palette();
    wallpaperPalette.setBrush(QPalette::Background, wallpaperPixmap.scaled(qApp->screens()[0]->size(), Qt::IgnoreAspectRatio));
    this->setWindowIcon(windowApp_icon);
    this->setPalette(wallpaperPalette);

    loginpage = new loginPage(this);
    loginpage->showFullScreen();
    this->hide();
    connect(loginpage, &loginPage::forcedClosed, this, &MyApp::close);


//Widgets
    appFeatures.append(new QAction("About System", this));
    appFeatures.append(new QAction("Log Out", this));
    appFeatures.append(new QAction("Shut Down", this));
    for(int i = 0; i<app_count; i++) Apps.append(new folderButton(this));

    QTimer *updater = new QTimer(this);                 //new

//Widgets setup (set the widget properties)
    appFeatures[0]->setStatusTip("Show the status of this OS");
    appFeatures[1]->setStatusTip("Log out from the current account");
    appFeatures[2]->setStatusTip("Shut down this computer");

    QMenuBar *dateTime = new QMenuBar(ui->menubar);                 //new start
    dateAction = new QAction(QDateTime::currentDateTime().date().toString(), this);
    timeAction = new QAction(QDateTime::currentDateTime().time().toString(), this);
    dateTime->addAction(dateAction);
    dateTime->addAction(timeAction);
    ui->menubar->setCornerWidget(dateTime, Qt::TopRightCorner);     //new end

    Apps[0]->renameFolder("File Manager");
    Apps[1]->renameFolder("Notepad");
    Apps[2]->renameFolder("Calculator");
    Apps[3]->renameFolder("Task Manager");
    Apps[4]->renameFolder("Calendar");
    Apps[5]->renameFolder("GPA Planner");
    Apps[6]->renameFolder("Kernel");
    Apps[7]->renameFolder("Clock");

    Apps[1]->setFileIcon(notepad_icon);
    Apps[2]->setFileIcon(calc_icon);
    Apps[3]->setFileIcon(taskM_icon);
    Apps[4]->setFileIcon(calendar_icon);
    Apps[5]->setFileIcon(GPAPlan_icon);
    Apps[6]->setFileIcon(kernel_icon);
    Apps[7]->setFileIcon(clock_icon);

    app_list = menuBar()->addMenu("");
    app_list->setIcon(applist_icon);
    appname = openedApps.keys();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    QMenu *app_name = menuBar()->addMenu(program_name);
    //here add app features (e.g. copy, close, etc)
    app_name->addAction(appFeatures[0]);
    app_name->addSeparator();
    app_name->addAction(appFeatures[1]);
    app_name->addAction(appFeatures[2]);

//Layouts (set the display layout)
    QWidget *file_space = new QWidget;
    QGridLayout *file_layout = new QGridLayout(file_space);
    file_layout->setAlignment(Qt::AlignLeft);
    //here add app name
    for(int i = 0; i<app_count; i++) file_layout->addWidget(Apps[i]);


    ui->main_layout->addWidget(file_space);
    ui->main_layout->addStretch(1);
    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &MyApp::switchApps);
    }
    //start here
    connect(appFeatures[0], &QAction::triggered, this, &MyApp::aboutSystemTrigg);
    connect(appFeatures[1], &QAction::triggered, this, &MyApp::logOut);
    connect(appFeatures[2], &QAction::triggered, this, &MyApp::close);
    connect(app_list, &QMenu::aboutToShow, this, &MyApp::appListUpdate);        //new
    connect(updater, &QTimer::timeout, this, &MyApp::updateTime);               //new
    updater->start(1000);                                                       //new
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(this, &MyApp::openClock, this, &MyApp::openClockAction);
    connect(this, &MyApp::openCalendar, this, &MyApp::openCalendarAction);
    for(int i = 0; i<app_count; i++) connect(Apps[i], &folderButton::fileOpened, this, &MyApp::openApp);
}

MyApp::~MyApp()
{
    delete ui;
}

void MyApp::aboutSystemTrigg(){
    aboutsystem = new aboutSystem(this);
    aboutsystem->showFullScreen();
    this->hide();
    connect(aboutsystem, &aboutSystem::openCalendar, this, &MyApp::openCalendarAction);
    connect(aboutsystem, &aboutSystem::openClock, this, &MyApp::openClockAction);
}

void MyApp::appListUpdate(){                                //new
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &MyApp::switchApps);
    }
}

void MyApp::logOut(){
        loginpage = new loginPage(this);
        loginpage->showFullScreen();
        this->hide();
        connect(loginpage, &loginPage::forcedClosed, this, &MyApp::close);
}

void MyApp::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void MyApp::updateTime(){                                             //new
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

//add function below
void MyApp::openApp(){
    folderButton *app = (folderButton*) sender();
    if(appname.contains(app->getName())) openedApps[app->getName()]->show();
    else if(app->getName() == "File Manager"){
        filemanager = new FileManager(this);
        filemanager->showFullScreen();
        connect(filemanager, &FileManager::openCalendar, this, &MyApp::openCalendarAction);
        connect(filemanager, &FileManager::openClock, this, &MyApp::openClockAction);
        connect(filemanager, SIGNAL(openNotepad(QString)), this, SLOT(openNotepad(QString)));
    }else if(app->getName() == "Notepad"){
        notepad = new Notepad(this);
        notepad->showFullScreen();
        connect(notepad, &Notepad::openCalendar, this, &MyApp::openCalendarAction);
        connect(notepad, &Notepad::openClock, this, &MyApp::openClockAction);
    }else if(app->getName() == "Calculator"){
        calc = new Calculator(this);
        calc->showFullScreen();
        connect(calc, &Calculator::openCalendar, this, &MyApp::openCalendarAction);
        connect(calc, &Calculator::openClock, this, &MyApp::openClockAction);
    }else if(app->getName() == "Task Manager"){
        taskmanager = new taskManager(this);
        taskmanager->showFullScreen();
        connect(taskmanager, &taskManager::openCalendar, this, &MyApp::openCalendarAction);
        connect(taskmanager, &taskManager::openClock, this, &MyApp::openClockAction);
    }else if(app->getName() == "Calendar"){
        calendar = new Calendar(this);
        calendar->showFullScreen();
        connect(calendar, &Calendar::openClock, this, &MyApp::openClockAction);
    }else if(app->getName() == "GPA Planner"){
        gpaplanner = new GPAPlanner(this);
        gpaplanner->showFullScreen();
        connect(gpaplanner, &GPAPlanner::openCalendar, this, &MyApp::openCalendarAction);
        connect(gpaplanner, &GPAPlanner::openClock, this, &MyApp::openClockAction);
    }else if(app->getName() == "Kernel"){
        kernel = new Kernel(this);
        kernel->showFullScreen();
        connect(kernel, &Kernel::openCalendar, this, &MyApp::openCalendarAction);
        connect(kernel, &Kernel::openClock, this, &MyApp::openClockAction);
        connect(kernel, SIGNAL(sendMessage(QString)), this, SLOT(openNotepad(QString)));
    }else if(app->getName() == "Clock"){
        clock = new Clock(this);
        clock->showFullScreen();
        connect(clock, &Clock::openCalendar, this, &MyApp::openCalendarAction);
    }
    this->hide();
}

void MyApp::closeEvent(QCloseEvent *event){
    exit(0);
}

void MyApp::openCalendarAction(){
    QWidget *triggApp = (QWidget*) sender();
    if(appname.contains("Calendar")){
        openedApps["Calendar"]->show();
    }else{
        calendar = new Calendar(this);
        calendar->showFullScreen();
        connect(calendar, &Calendar::openClock, this, &MyApp::openClockAction);
    }
    triggApp->hide();
}

void MyApp::openClockAction(){
    QWidget *triggApp = (QWidget*) sender();
    if(appname.contains("Clock")){
        openedApps["Clock"]->show();
    }else{
        clock = new Clock(this);
        clock->showFullScreen();
        connect(clock, &Clock::openCalendar, this, &MyApp::openCalendarAction);
    }
    triggApp->hide();
}

void MyApp::openNotepad(QString path){
    QWidget *reqApp = (QWidget*) sender();
    if(appname.contains("Notepad")){
    QMessageBox::StandardButton choice = QMessageBox::question(this, "Unsaved Changes", "Notepad is currently opening another text file. Opening this file will reopen the Notepad. Unsaved changes will be lost. Do you wish to continue?");
        if(choice == QMessageBox::Yes){
            notepad->openFromOutside(path);
            notepad->show();
            reqApp->hide();
        }else{
            this->show();
            reqApp->show();
            this->hide();
        }
    }else{
        notepad = new Notepad(this);
        notepad->showFullScreen();
        reqApp->hide();
        connect(notepad, &Notepad::openCalendar, this, &MyApp::openCalendarAction);
        connect(notepad, &Notepad::openClock, this, &MyApp::openClockAction);
        notepad->openFromOutside(path);
    }
}
