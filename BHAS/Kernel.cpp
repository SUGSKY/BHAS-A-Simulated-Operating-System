#include <string>
#include <sstream>
#include <algorithm>
#include<QDateTime>
#include<QDebug>
#include <QDir>
#include<QIcon>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include <QString>
#include<QTimer>
#include<QVector>
#include "Kernel.h"
#include "ui_Kernel.h"

using namespace std;

// change base folder here
const QString program_name = "Kernel";
QString folderName = QDir::currentPath() + QDir::separator() + "DATA";

int myCount = 0;
void makeSure(QDir dirq, QString s) {
    if (!dirq.exists()) {
        dirq.mkpath(s);
    }
}

QString duplicate = folderName;
int baseLength = folderName.length();

QDir dir(duplicate);
Kernel::Kernel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Kernel)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->insertPlainText(QString::fromStdString("baseFolder>"));
    ui->label->setText("baseFolder");
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(8);
    currAppUsage.append(6);
    currAppUsage.append(13);
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
    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Kernel::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &Kernel::close_app);
    connect(app_list, &QMenu::aboutToShow, this, &Kernel::appListUpdate);
    connect(updater, &QTimer::timeout, this, &Kernel::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    //start below
}

Kernel::~Kernel()
{
    delete ui;
}

void Kernel::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Kernel::switchApps);
    }
}

void Kernel::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void Kernel::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void Kernel::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    this->close();
}

void Kernel::on_lineEdit_returnPressed() {
    myCount+=1;
    if (myCount == 1) {
        makeSure(dir, duplicate);
        QDir dir0(duplicate);
        dir = dir0;
    }
    QString qcommand = ui->lineEdit->text();
    string command = qcommand.toStdString();
    ui->lineEdit->setText("");
    ui->textEdit->insertPlainText(qcommand + "\n");
    if (command.find('&') != -1) {
        ui->textEdit->insertPlainText("  Please input one command at a time\n");
    }
    else if (command.substr(0,3) == "md ") {

        string newFolder = command.substr(3,command.length()-3);

        if (newFolder.find('/') != -1) {
            ui->textEdit->insertPlainText("  invalid folder name\n");
        }
        else {
            QDir dir2(dir.path() + "/" + QString::fromStdString(newFolder));
            if (dir2.exists()) {
                ui->textEdit->insertPlainText("  folder already existed\n");
            }
            else {
                dir.mkdir(QString::fromStdString(newFolder));
            }
        }
    }
    else if (command.substr(0,5) == "cd ..") {
        if (dir.path().length() == baseLength) {}
        else {
            if(dir.cdUp()) {}
        }
    }
    else if (command.substr(0,3) == "cd ") {
        string newFolder = command.substr(3,command.length()-3);
        if (newFolder.find("/") != -1) {
            ui->textEdit->insertPlainText("  Please enter 1 folder at a time\n");
        }
        else {
            if (dir.cd(dir.path() + "/" + QString::fromStdString(newFolder))) {}
            else {
                ui->textEdit->insertPlainText("  Cannot find that\n");
            }
        }
    }
    else if (command.length() == 3 and command.substr(0,3) == "dir") {
        int counter = 1;
        dir.setPath(dir.path());
        foreach (QFileInfo var, dir.entryInfoList()) {
            if (counter >= 3) {
                ui->textEdit->insertPlainText("  " + var.fileName() + "\n");
            }
            counter += 1;
        }
    }
    else if (command.substr(0,3) == "rd ") {
        string newFolder = command.substr(3,command.length()-3);
        QDir dir3(dir.path() + "/" + QString::fromStdString(newFolder));
        if (!dir3.exists()) {
            ui->textEdit->insertPlainText("  Cannot find that\n");
        }
        else if (dir3.removeRecursively()) {

        }
        else {
            ui->textEdit->insertPlainText("  Please try again\n");
        }
    }
    else if (command.substr(0,5) == "echo ") {
        ui->textEdit->insertPlainText(QString::fromStdString(command.substr(5,command.length()-5))+"\n");
    }
    else if (command.length()>4){
        if (command.substr(command.length()-4,4) == ".txt") {
            if (QFileInfo(dir.path()+"/"+QString::fromStdString(command)).exists()) {
                ui->textEdit->insertPlainText("  Opening " + QString::fromStdString(command) + "...\n");
                emit sendMessage(dir.path()+"/"+QString::fromStdString(command));
            } else {
                ui->textEdit->insertPlainText("  Cannot find that\n");
            }

        }
    }
    else {

    }

    QString qcurrent = dir.path().mid(baseLength,dir.path().length()+1-baseLength);
    qcurrent = "baseFolder" + qcurrent;
    ui->label->setText(qcurrent);
    ui->textEdit->insertPlainText(qcurrent + ">");
}
