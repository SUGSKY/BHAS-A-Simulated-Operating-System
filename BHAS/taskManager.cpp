#include<QDateTime>
#include<QDebug>
#include<QIcon>
#include<QMessageBox>
#include<QPalette>
#include<QPixmap>
#include<QRandomGenerator>
#include<QScreen>
#include<QSize>
#include<QTimer>
#include<QVector>
#include "taskManager.h"
#include "ui_taskManager.h"

//Global Variables (global variables for this app only)
const QString program_name = "Task Manager";
const int dev = 2;
//start below


taskManager::taskManager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::taskManager)
{
    ui->setupUi(this);
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(7);
    currAppUsage.append(10);
    currAppUsage.append(6);
    appUsage[program_name] = currAppUsage;

//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    //start below



//Widgets (initialization for widget)
    //start below
    appFeatures.append(new QAction("Close", this));                 //add features (please modify)
    QTimer *updater = new QTimer(this);

    CPULevel = new QProgressBar(this);
    RAMLevel = new QProgressBar(this);
    DISKLevel = new QProgressBar(this);
    QLabel *appListText = new QLabel(this);
    QLabel *appCPUText = new QLabel(this);
    QLabel *appRAMText = new QLabel(this);
    QLabel *appDISKText = new QLabel(this);
    QLabel *CPULevelText = new QLabel(this);
    QLabel *RAMLevelText = new QLabel(this);
    QLabel *DISKLevelText = new QLabel(this);
    QFrame *verticalLine = new QFrame(this);
    QTimer *updateStatusTimer = new QTimer(this);
    QSpacerItem *verticalSpacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding);





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

    appListText->setText("Active Application(s):");
    appListText->setFixedSize(270,35);
    appListText->setAlignment(Qt::AlignLeft);
    appCPUText->setText("CPU");
    appCPUText->setFixedSize(120,35);
    appCPUText->setAlignment(Qt::AlignCenter);
    appRAMText->setText("Memory");
    appRAMText->setFixedSize(120,35);
    appRAMText->setAlignment(Qt::AlignCenter);
    appDISKText->setText("Disk");
    appDISKText->setFixedSize(120,35);
    appDISKText->setAlignment(Qt::AlignCenter);
    CPULevel->setRange(0,100);
    CPULevel->setValue(100);
    CPULevel->setOrientation(Qt::Vertical);
    CPULevel->setFixedSize(100,720);
    CPULevel->setInvertedAppearance(1);
    CPULevel->setStyleSheet("QProgressBar:vertical {border: 1px solid gray;border-radius: 3px;background: QLinearGradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 green, stop: 0.6 green,stop: 0.8 orange, stop: 1 red);padding: 0px;} QProgressBar::chunk:vertical {background:gray}");
    RAMLevel->setRange(0,100);
    RAMLevel->setValue(100);
    RAMLevel->setOrientation(Qt::Vertical);
    RAMLevel->setFixedSize(100,720);
    RAMLevel->setInvertedAppearance(1);
    RAMLevel->setStyleSheet("QProgressBar:vertical {border: 1px solid gray;border-radius: 3px;background: QLinearGradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 green, stop: 0.6 green,stop: 0.8 orange, stop: 1 red);padding: 0px;} QProgressBar::chunk:vertical {background:gray}");
    DISKLevel->setRange(0,100);
    DISKLevel->setValue(100);
    DISKLevel->setOrientation(Qt::Vertical);
    DISKLevel->setFixedSize(100,720);
    DISKLevel->setInvertedAppearance(1);
    DISKLevel->setStyleSheet("QProgressBar:vertical {border: 1px solid gray;border-radius: 3px;background: QLinearGradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 green, stop: 0.6 green,stop: 0.8 orange, stop: 1 red);padding: 0px;} QProgressBar::chunk:vertical {background:gray}");
    CPULevelText->setText("CPU Usage");
    RAMLevelText->setText("Memory Usage");
    DISKLevelText->setText("Disk Usage");
    verticalLine->setFrameShape(QFrame::VLine);
    verticalLine->setFixedSize(10, qApp->screens()[0]->size().height()-150);


//Layouts (set the display layout)
    taskListWidget = new QWidget;
    taskListLayout = new QGridLayout(taskListWidget);
    taskListWidget->setFixedSize((qApp->screens()[0]->size().width()-100)/2, qApp->screens()[0]->size().height()-150);
    taskListLayout->setContentsMargins(0,0,0,0);
    taskListLayout->setVerticalSpacing(10);
    taskListLayout->addWidget(appListText,0,0,Qt::AlignLeft | Qt::AlignTop);
    taskListLayout->addWidget(appCPUText,0,1,Qt::AlignHCenter | Qt::AlignTop);
    taskListLayout->addWidget(appRAMText,0,2,Qt::AlignHCenter | Qt::AlignTop);
    taskListLayout->addWidget(appDISKText,0,3,Qt::AlignHCenter | Qt::AlignTop);
    taskListLayout->addItem(verticalSpacer, 10,0);
    taskListLayout->addItem(verticalSpacer, 10,1);
    taskListLayout->addItem(verticalSpacer, 10,2);
    taskListLayout->addItem(verticalSpacer, 10,3);

    graphsCPULevelWidget = new QWidget;
    graphsCPULevelLayout = new QVBoxLayout(graphsCPULevelWidget);
    graphsCPULevelLayout->addWidget(CPULevel, 0, Qt::AlignCenter);
    graphsCPULevelLayout->addWidget(CPULevelText, 0, Qt::AlignCenter);

    graphsRAMLevelWidget = new QWidget;
    graphsRAMLevelLayout = new QVBoxLayout(graphsRAMLevelWidget);
    graphsRAMLevelLayout->addWidget(RAMLevel, 0, Qt::AlignCenter);
    graphsRAMLevelLayout->addWidget(RAMLevelText, 0, Qt::AlignCenter);

    graphsDISKLevelWidget = new QWidget;
    graphsDISKLevelLayout = new QVBoxLayout(graphsDISKLevelWidget);
    graphsDISKLevelLayout->addWidget(DISKLevel, 0, Qt::AlignCenter);
    graphsDISKLevelLayout->addWidget(DISKLevelText, 0, Qt::AlignCenter);

    QWidget *graphsLevelWidget = new QWidget;
    QHBoxLayout *graphsLevelLayout = new QHBoxLayout(graphsLevelWidget);
    graphsLevelWidget->setFixedSize((qApp->screens()[0]->size().width()-100)/2, qApp->screens()[0]->size().height()-150);
    graphsLevelLayout->addWidget(graphsCPULevelWidget, 0, Qt::AlignCenter);
    graphsLevelLayout->addWidget(graphsRAMLevelWidget, 0, Qt::AlignCenter);
    graphsLevelLayout->addWidget(graphsDISKLevelWidget, 0, Qt::AlignCenter);

    ui->mainLayout->addSpacing(35);
    ui->mainLayout->addWidget(taskListWidget, 0, Qt::AlignLeft | Qt::AlignVCenter);
    ui->mainLayout->addWidget(verticalLine, 0, Qt::AlignCenter);
    ui->mainLayout->addWidget(graphsLevelWidget, 0, Qt::AlignLeft);
    //don't add code here

//Connection (set signal and slot)
    connect(app_list, &QMenu::aboutToShow, this, &taskManager::appListUpdate);
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &taskManager::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &taskManager::close_app);
    connect(updater, &QTimer::timeout, this, &taskManager::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    connect(updateStatusTimer, &QTimer::timeout, this, &taskManager::updateStatus);
    updateStatusTimer->start(1000);
    //start below


}

taskManager::~taskManager()
{
    delete ui;
}

void taskManager::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &taskManager::switchApps);
    }
}

void taskManager::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void taskManager::updateStatus(){
    int temp, totalCPU = 0, totalRAM = 0, totalDISK = 0;
    for(int i = currTask.size(); i<appname.size(); i++){
        currTask.append(new QLabel(this));
        currTask[i]->setFixedSize(270,35);
        currTask[i]->setAlignment(Qt::AlignLeft);
        taskListLayout->addWidget(currTask[i], i+1, 0, Qt::AlignTop | Qt::AlignLeft);
        currTaskCPU.append(new QLabel(this));
        currTaskCPU[i]->setFixedSize(120,35);
        currTaskCPU[i]->setAlignment(Qt::AlignCenter);
        taskListLayout->addWidget(currTaskCPU[i], i+1, 1, Qt::AlignTop | Qt::AlignHCenter);
        currTaskRAM.append(new QLabel(this));
        currTaskRAM[i]->setFixedSize(120,35);
        currTaskRAM[i]->setAlignment(Qt::AlignCenter);
        taskListLayout->addWidget(currTaskRAM[i], i+1, 2, Qt::AlignTop | Qt::AlignHCenter);
        currTaskDISK.append(new QLabel(this));
        currTaskDISK[i]->setFixedSize(120,35);
        currTaskDISK[i]->setAlignment(Qt::AlignCenter);
        taskListLayout->addWidget(currTaskDISK[i], i+1, 3, Qt::AlignTop | Qt::AlignHCenter);
        endTaskButton.append(new QPushButton("End Application", this));
        endTaskButton[i]->setFixedSize(120,35);
        endTaskButton[i]->setObjectName(QString::number(i));
        connect(endTaskButton[i], &QPushButton::clicked, this, &taskManager::endTask);
        taskListLayout->addWidget(endTaskButton[i], i+1, 4, Qt::AlignTop | Qt::AlignHCenter);
    }
    for(int i = 0; i<currTask.size(); i++){
        if(i>=appname.size()){
            currTask[i]->hide();
            currTaskCPU[i]->hide();
            currTaskRAM[i]->hide();
            currTaskDISK[i]->hide();
            endTaskButton[i]->hide();
            continue;
        }
        if(currTask[i]->isHidden()) currTask[i]->show();
        currTask[i]->setText(appname[i]);
        temp = int(QRandomGenerator::global()->generateDouble()*2*dev+appUsage[appname[i]][0]-dev);
        totalCPU += temp;
        if(currTaskCPU[i]->isHidden()) currTask[i]->show();
        currTaskCPU[i]->setText(QString::number(temp) + "%");
        temp = int(QRandomGenerator::global()->generateDouble()*2*dev+appUsage[appname[i]][1]-dev);
        totalRAM += temp;
        if(currTaskRAM[i]->isHidden()) currTask[i]->show();
        currTaskRAM[i]->setText(QString::number(temp) + "%");
        temp = int(QRandomGenerator::global()->generateDouble()*2*dev+appUsage[appname[i]][2]-dev);
        totalDISK += temp;
        if(currTaskDISK[i]->isHidden()) currTask[i]->show();
        currTaskDISK[i]->setText(QString::number(temp) + "%");
        if(endTaskButton[i]->isHidden()) endTaskButton[i]->show();
    }
    CPULevel->setValue(100-totalCPU);
    CPULevel->setFormat(QString::number(100-CPULevel->value()) + "%");
    RAMLevel->setValue(100-totalRAM);
    RAMLevel->setFormat(QString::number(100-RAMLevel->value()) + "%");
    DISKLevel->setValue(100-totalDISK);
    DISKLevel->setFormat(QString::number(100-DISKLevel->value()) + "%");
}

void taskManager::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void taskManager::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    this->close();
}

void taskManager::endTask(){
    QPushButton *killedAppButton = (QPushButton*) sender();
    int idx = killedAppButton->objectName().toInt();
    QString killedAppName = appname[idx];
    if(killedAppName == program_name) close_app();
    else if (killedAppName == "Desktop"){
        QMessageBox::StandardButton answer = QMessageBox::question(this, "Ending Desktop Task", "Ending desktop task means that this OS will be shutted down. Do you still wish to proceed?");
        if(answer == QMessageBox::Yes){
            exit(0);
        }
    }else{
        appname.removeOne(killedAppName);
        openedApps.remove(killedAppName);
    }
}

void taskManager::closeEvent(QCloseEvent *event){
    close_app();
}

//add function below





