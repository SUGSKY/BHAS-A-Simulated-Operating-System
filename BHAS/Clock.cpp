#include<QDateTime>
#include<QDebug>
#include<QIcon>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include<QVector>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QLabel>
#include <QtGui>
#include <QTabWidget>
#include <QTimeEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QPushButton>
#include <QLCDNumber>
#include "Clock.h"
#include "ui_Clock.h"

//Global Variables (global variables for this app only)
const QString program_name = "Clock";
//start below
static QPushButton *start, *pause, *rst;
static bool running;
static qint64 totalTime, sessionTime;
static QDateTime startTime;
static QSpinBox *insHour, *insMinute, *insSecond;
static QLabel *stopwatchDisplay, *timerDisplay;
static QTime countdown(0,0,0);
static QPushButton *startbtn, *resetbtn;
static QTimer *time2;

Clock::Clock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clock)
{
    ui->setupUi(this);
    openedApps[program_name] = this;              //don't delete
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(5);
    currAppUsage.append(7);
    currAppUsage.append(3);
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
    appname = openedApps.keys();                                  //don't delete
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    QMenu *app_name = menuBar()->addMenu(program_name);
    //here add app features (e.g. copy, close, etc)
    app_name->addAction(appFeatures[0]);                            //add feature 0 to layout (please modify)

//Layouts (set the display layout)
    QWidget *file_space = new QWidget;
    //here add app name                                             //main content of your app
    QVBoxLayout *main = new QVBoxLayout(file_space);
    QHBoxLayout *layout1 = new QHBoxLayout;
    QVBoxLayout *layout2 = new QVBoxLayout;
    analogClock *aClock = new analogClock();
    digitalClock *dClock = new digitalClock();

    QGroupBox *stopwatch = new QGroupBox("Stopwatch");
    QGroupBox *timer = new QGroupBox("Timer");

    layout1->addWidget(aClock);
    layout1->addWidget(dClock);

    layout2->addWidget(stopwatch);
    QGridLayout *stopwatchLayout = new QGridLayout;
    start = new QPushButton("Start");
    pause = new QPushButton("Pause");
    rst = new QPushButton("Reset");
    start->setEnabled(true);
    pause->setEnabled(false);
    rst->setEnabled(false);
    stopwatchDisplay = new QLabel("00:00:00,000");
    QFont font("Arial", 24, QFont::Bold);
    stopwatchDisplay->setFont(font);
    stopwatchLayout->addWidget(stopwatchDisplay,0,0,1,3);
    stopwatchLayout->addWidget(start,1,0,1,1);
    stopwatchLayout->addWidget(pause,1,1,1,1);
    stopwatchLayout->addWidget(rst,1,2,1,1);
    stopwatch->setLayout(stopwatchLayout);
    running = false;
    totalTime = 0;

    layout2->addWidget(timer);
    QGridLayout *timerLayout = new QGridLayout;   
    timerDisplay = new QLabel(countdown.toString());
    QFont font2("Arial", 24, QFont::Bold);
    timerDisplay->setFont(font2);
    timerLayout->addWidget(timerDisplay,0,0,1,3);
    QLabel *label5 = new QLabel("Hour");
    QLabel *label6 = new QLabel("Minute");
    QLabel *label7 = new QLabel("Second");
    timerLayout->addWidget(label5,1,0,1,1);
    timerLayout->addWidget(label6,1,1,1,1);
    timerLayout->addWidget(label7,1,2,1,1);

    insHour = new QSpinBox;
    insMinute = new QSpinBox;
    insSecond = new QSpinBox;
    timerLayout->addWidget(insHour,2,0,1,1);
    timerLayout->addWidget(insMinute,2,1,1,1);
    timerLayout->addWidget(insSecond,2,2,1,1);
    insHour->setMaximum(23);
    insMinute->setMaximum(59);
    insSecond->setMaximum(59);

    startbtn = new QPushButton("Start");
    resetbtn = new QPushButton("Reset");
    timerLayout->addWidget(startbtn,3,0,1,1);
    timerLayout->addWidget(resetbtn,3,1,1,1);
    startbtn->setEnabled(true);
    resetbtn->setEnabled(false);

    timer->setLayout(timerLayout);

    main->addLayout(layout1);
    main->addLayout(layout2);
    file_space->setLayout(main);

    ui->main_layout->addWidget(file_space);
    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){                                        //don't delete
        connect(appList[i], &QAction::triggered, this, &Clock::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &Clock::close_app);
    connect(app_list, &QMenu::aboutToShow, this, &Clock::appListUpdate);
    connect(updater, &QTimer::timeout, this, &Clock::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    //start below
    connect(start,&QPushButton::clicked,this,&Clock::stopwatchStart);
    connect(pause,&QPushButton::clicked,this,&Clock::stopwatchPause);
    connect(rst,&QPushButton::clicked,this,&Clock::stopwatchReset);
    startTimer(0);

    connect(startbtn,&QPushButton::clicked,this,&Clock::timerStart);
    connect(resetbtn,&QPushButton::clicked,this,&Clock::timerReset);
}

Clock::~Clock()
{
    delete ui;
}

void Clock::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Clock::switchApps);
    }
}

analogClock::analogClock(QWidget *parent) : QWidget(parent){
    QTimer *time = new QTimer(this);
    connect(time,&QTimer::timeout,this,&analogClock::showTime);
    time->start(1000);
}

digitalClock::digitalClock(QWidget *parent) : QLCDNumber(parent){
    setSegmentStyle(Filled);
    QTimer *time = new QTimer(this);
    connect(time,&QTimer::timeout,this,&digitalClock::showTime);
    time->start(1000);
    showTime();
}

void Clock::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){                  //don't delete
        openedApps[app->text()]->show();
        this->hide();
    }
}

void Clock::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void Clock::close_app(){
    appname.removeOne(program_name);                      //don't delete
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    this->close();
}

//add function below

void analogClock::showTime(){
    QWidget::update();
}

void digitalClock::showTime(){
    QTime curTime = QTime::currentTime();
    QString x = curTime.toString("hh:mm");
    if ((curTime.second() % 2) == 0){
        x[2] = ' ';
    }
    display(x);
}

void analogClock::paintEvent(QPaintEvent *){
    static const QPoint hour[3] = {QPoint(7,8), QPoint(-7,8), QPoint(0,-40)};
    static const QPoint minute[3] = {QPoint(7,8), QPoint(-7,8), QPoint(0,-70)};
    QColor hourColor(0,0,1);
    QColor minuteColor(0,50,0,180);
    int x = qMin(width(),height());
    QPainter draw(this);
    QTime curTime = QTime::currentTime();

    draw.setRenderHint(QPainter::Antialiasing);
    draw.translate(width()/2, height()/2);
    draw.scale(x/200.0, x/200.0);
    draw.setPen(Qt::NoPen);
    draw.setBrush(hourColor);

    draw.save();
    draw.rotate(30.0*((curTime.hour()+curTime.minute()/60.0)));
    draw.drawConvexPolygon(hour,3);
    draw.restore();
    draw.setPen(hourColor);
    for(int i=0; i<12; i++){
        draw.drawLine(88,0,96,0);
        draw.rotate(30.0);
    }

    draw.setPen(Qt::NoPen);
    draw.setBrush(minuteColor);
    draw.save();
    draw.rotate(6.0*(curTime.minute()+curTime.second()/60.0));
    draw.drawConvexPolygon(minute,3);
    draw.restore();
    draw.setPen(minuteColor);
    for(int j=0; j<60; j++){
        if(j%5 != 0){
            draw.drawLine(92,0,96,0);
        }
        draw.rotate(6.0);
    }
}

void Clock::stopwatchStart(void){
    startTime = QDateTime::currentDateTime();
    running = true;
    start->setEnabled(false);
    pause->setEnabled(true);
    rst->setEnabled(true);
}

void Clock::stopwatchPause(void){
    start->setEnabled(true);
    pause->setEnabled(false);
    rst->setEnabled(true);
    timerEvent(new QTimerEvent(0));
    totalTime += sessionTime;
    running = false;
}

void Clock::stopwatchReset(void){
    start->setEnabled(true);
    pause->setEnabled(false);
    rst->setEnabled(false);
    totalTime = 0;
    running = false;
    stopwatchDisplay->setText("00:00:00,000");
}

void Clock::timerEvent(QTimerEvent *){
    if(running){
        sessionTime = startTime.msecsTo(QDateTime::currentDateTime());
        qint64 time1 = totalTime + sessionTime;
        long long hour = time1/1000/60/60;
        long long minute = (time1/1000/60)-(hour*60);
        long long second = (time1/1000)-((minute+(hour*60))*60);
        long long msecond = time1-(second+((minute+(hour*60))*60))*1000;
        const QString x = QString("%1:%2:%3,%4").arg(hour,2,10,QChar('0'))
                                                .arg(minute,2,10,QChar('0'))
                                                .arg(second,2,10,QChar('0'))
                                                .arg(msecond,3,10,QChar('0'));
        stopwatchDisplay->setText(x);
    }
}

void Clock::timerStart(){
    startbtn->setEnabled(false);
    resetbtn->setEnabled(true);
    countdown.setHMS(insHour->value(),insMinute->value(),insSecond->value()+1);
    time2 = new QTimer;
    connect(time2,&QTimer::timeout,this,&Clock::updateTimer);
    time2->start(1000);
}

void Clock::timerReset(){
    startbtn->setEnabled(true);
    resetbtn->setEnabled(false);
    time2->stop();
    countdown.setHMS(0,0,0);
    timerDisplay->setText(countdown.toString());
}

void Clock::updateTimer(){
    countdown.setHMS(countdown.addSecs(-1).hour(),countdown.addSecs(-1).minute(),countdown.addSecs(-1).second());
    timerDisplay->setText(countdown.toString());
    if(countdown.hour()==0 && countdown.minute()==0 && countdown.second()==0){
        time2->stop();
        timerReset();
    }
}






