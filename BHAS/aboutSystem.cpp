#include<QDateTime>
#include<QDebug>
#include<QHBoxLayout>
#include<QIcon>
#include<QLabel>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include<QSysInfo>
#include<QTimer>
#include<QVBoxLayout>
#include<QVector>
#include "aboutSystem.h"
#include "ui_aboutSystem.h"

//Global Variables (global variables for this app only)
const QString program_name = "About System";
const QString BASHVersion = "Bash 7.3.0";


aboutSystem::aboutSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::aboutSystem)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(3);
    currAppUsage.append(3);
    currAppUsage.append(3);
    appUsage[program_name] = currAppUsage;

//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    QPixmap LOGO_icon(":/img/LOGO.png");
    //start below



//Widgets (initialization for widget)
    //start below
    appFeatures.append(new QAction("Close", this));

    QTimer *updater = new QTimer(this);

    QLabel *LOGO = new QLabel(this);
    QLabel *BEText = new QLabel(this);
    QLabel *BV = new QLabel(this);
    QLabel *SysText = new QLabel(this);
    QLabel *SysType = new QLabel(this);
    QLabel *ComNameText = new QLabel(this);
    QLabel *ComName = new QLabel(this);


//Widgets setup (set the widget properties)
    //start below
    appFeatures[0]->setStatusTip("Close application");

    QMenuBar *dateTime = new QMenuBar(ui->menubar);
    dateAction = new QAction(QDateTime::currentDateTime().date().toString(), this);
    timeAction = new QAction(QDateTime::currentDateTime().time().toString(), this);
    dateTime->addAction(dateAction);
    dateTime->addAction(timeAction);
    ui->menubar->setCornerWidget(dateTime, Qt::TopRightCorner);

    LOGO->setFixedSize(800,533);
    LOGO->setPixmap(LOGO_icon.scaled(800, 533, Qt::KeepAspectRatio));
    BEText->setText("Bash Edition: ");
    BEText->setFixedSize(150, 35);
    BV->setText(BASHVersion);
    SysText->setText("System Type: ");
    SysText->setFixedSize(150, 35);
    SysType->setText(QSysInfo::currentCpuArchitecture());
    ComNameText->setText("Computer Name: ");
    ComNameText->setFixedSize(150, 35);
    ComName->setText(QSysInfo::machineHostName());


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



//Layouts (set the display layout)
    QWidget *infoWidget = new QWidget;
    QGridLayout *infoLayout = new QGridLayout(infoWidget);
    infoLayout->addWidget(BEText,0,0,Qt::AlignLeft);
    infoLayout->addWidget(BV,0,1,Qt::AlignLeft);
    infoLayout->addWidget(SysText,1,0,Qt::AlignLeft);
    infoLayout->addWidget(SysType,1,1,Qt::AlignLeft);
    infoLayout->addWidget(ComNameText,2,0,Qt::AlignLeft);
    infoLayout->addWidget(ComName,2,1,Qt::AlignLeft);

    ui->main_layout->addWidget(LOGO, 0, Qt::AlignTop | Qt::AlignHCenter);
    ui->main_layout->addWidget(infoWidget,0, Qt::AlignCenter);
    ui->main_layout->addStretch(1);
    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &aboutSystem::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &aboutSystem::close_app);
    connect(app_list, &QMenu::aboutToShow, this, &aboutSystem::appListUpdate);
    connect(updater, &QTimer::timeout, this, &aboutSystem::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    //start below


}

aboutSystem::~aboutSystem()
{
    delete ui;
}

void aboutSystem::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &aboutSystem::switchApps);
    }
}

void aboutSystem::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void aboutSystem::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void aboutSystem::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    this->close();
}

//add function below





