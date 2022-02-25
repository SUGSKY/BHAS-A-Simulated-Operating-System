#include<QDateTime>
#include<QDebug>
#include<QIcon>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include<QVector>
#include <QCalendarWidget>
#include <QGroupBox>
#include <QSizePolicy>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTimeEdit>
#include <QTimer>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QString>
#include "Calendar.h"
#include "ui_Calendar.h"

//Global Variables (global variables for this app only)
const QString program_name = "Calendar";
//start below
static QLineEdit *eventTitle;
static QTimeEdit *eventTime;
static QTextEdit *eventDetail;
static QListWidget *eventList;
static QCalendarWidget *cal;
static QPushButton *delButton;
static QMultiMap<QDate,QMap<QString,QString>> storeEvent = {
};

Calendar::Calendar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calendar)
{
    ui->setupUi(this);
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(4);
    currAppUsage.append(7);
    currAppUsage.append(9);
    appUsage[program_name] = currAppUsage;

//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    //start below

//Widgets (initialization for widget)
    //start below
    appFeatures.append(new QAction("Close", this));                 //add features (please modify)
    QTimer *updater = new QTimer(this);

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
    QWidget *file_space = new QWidget;
    QHBoxLayout *main = new QHBoxLayout(file_space);
    main->setAlignment(Qt::AlignLeft);
    //here add app name                                             //main content of your app
    cal = new QCalendarWidget;
    cal->setMaximumDate(QDate(2200,1,1));
    cal->setMinimumDate(QDate(1900,1,1));
    cal->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    main->addWidget(cal);

    QVBoxLayout *right = new QVBoxLayout;
    eventList = new QListWidget;
    QLabel *label = new QLabel("Events On Date");
    right->addWidget(label);
    right->addWidget(eventList);
    main->addLayout(right);
    eventList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGroupBox *eventForm = new QGroupBox("Event");
    right->addWidget(eventForm);
    QGridLayout *formLayout = new QGridLayout;
    eventForm->setLayout(formLayout);

    eventTitle = new QLineEdit;
    eventTime = new QTimeEdit(QTime(7,0));
    eventDetail = new QTextEdit();
    QPushButton *addButton = new QPushButton("Add / Update");
    delButton = new QPushButton("Delete");

    QLabel *label2 = new QLabel("Note");
    formLayout->addWidget(eventTitle,1,1,1,3);
    formLayout->addWidget(eventTime,2,1);
    formLayout->addWidget(label2,3,1);
    formLayout->addWidget(eventDetail,4,1,1,3);
    formLayout->addWidget(addButton,5,2);
    formLayout->addWidget(delButton,5,3);

    ui->main_layout->addWidget(file_space);
    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Calendar::switchApps);
    }
    connect(app_list, &QMenu::aboutToShow, this, &Calendar::appListUpdate);
    connect(updater, &QTimer::timeout, this, &Calendar::updateTime);
    updater->start(1000);
    connect(appFeatures[0], &QAction::triggered, this, &Calendar::close_app);
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    //start below
    checkDeleteButton();
    connect(cal,&QCalendarWidget::selectionChanged,this,&Calendar::showEvent);
    connect(cal,&QCalendarWidget::selectionChanged,this,&Calendar::checkDeleteButton);
    connect(addButton,&QPushButton::clicked,this,&Calendar::saveEvent);
    connect(addButton,&QPushButton::clicked,this,&Calendar::checkDeleteButton);
    connect(delButton,&QPushButton::clicked,this,&Calendar::deleteEvent);
    connect(eventList,&QListWidget::itemSelectionChanged,this,&Calendar::checkDeleteButton);
    connect(eventList,&QListWidget::itemPressed,this,&Calendar::showDetails);
    connect(eventTitle,&QLineEdit::editingFinished,this,&Calendar::clearDetails);
}

Calendar::~Calendar()
{
    delete ui;
}

void Calendar::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Calendar::switchApps);
    }
}

void Calendar::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void Calendar::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void Calendar::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    this->close();
}

//add function below
void Calendar::clearForm(){
    eventTitle->clear();
    eventTime->setTime(QTime(7,0));
    eventDetail->setPlainText("");
}

void Calendar::showEvent(){
    eventList->clear();
    clearForm();
    QDate date = cal->selectedDate();
    QList<QMap<QString,QString>> temp = storeEvent.values(date);
    for(int i=0; i<temp.size(); i++){
        QString title = temp[i].value("Title");
        QString time = temp[i].value("Time");
        QString combined = time + " : " + title;
        eventList->addItem(combined);
    }
}

void Calendar::saveEvent(){
    QMap<QString,QString> event = {
      {"Title",eventTitle->text()},
      {"Time",eventTime->time().toString()},
      {"Details",eventDetail->toPlainText()}
    };
    QDate date = cal->selectedDate();
    storeEvent.insert(date,event);
    showEvent();
}

void Calendar::deleteEvent(){
    QDate date = cal->selectedDate();
    int row = eventList->currentRow();
    QList<QMap<QString,QString>> temp = storeEvent.values(date);
    QMap<QString,QString> idx = temp[row];
    storeEvent.erase(storeEvent.find(date,idx));
    eventList->setCurrentRow(-1);
    clearForm();
    showEvent();
}

void Calendar::showDetails(){
    clearForm();
    QDate date = cal->selectedDate();
    int row = eventList->currentRow();
    if(row == -1){
        return;
    }
    QList<QMap<QString,QString>> temp = storeEvent.values(date);
    QMap<QString,QString> idx = temp[row];
    eventDetail->setPlainText(idx["Details"]);
}

void Calendar::clearDetails(){
    eventDetail->setPlainText("");
}

void Calendar::checkDeleteButton(){
    delButton->setDisabled(eventList->currentRow()==-1);
}






