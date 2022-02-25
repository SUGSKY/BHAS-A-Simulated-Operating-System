#ifndef MYAPP_H
#define MYAPP_H

#include <QMainWindow>
#include <QMap>
#include"folderbutton.h"
#include"Global.h"

#include"aboutSystem.h"
#include"loginpage.h"
#include"Calc.h"
#include"Calendar.h"
#include"Clock.h"
#include"filemanager.h"
#include"GPAPlanner.h"
#include"Kernel.h"
#include"Notepad.h"
#include"taskManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyApp; }
QT_END_NAMESPACE

class MyApp : public QMainWindow
{
    Q_OBJECT

public:
    MyApp(QWidget *parent = nullptr);
    ~MyApp();
    void shutDownOS();

signals:
    void openClock();
    void openCalendar();

private:
    Ui::MyApp *ui;
    QMenu *app_list;                    //new
    QAction *dateAction;                //new
    QAction *timeAction;                //new
    QVector<folderButton*> Apps;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;

    aboutSystem *aboutsystem;
    loginPage *loginpage;
    FileManager *filemanager;
    Notepad *notepad;
    Calculator *calc;
    taskManager *taskmanager;
    Calendar *calendar;
    GPAPlanner *gpaplanner;
    Kernel *kernel;
    Clock *clock;


    void aboutSystemTrigg();
    void appListUpdate();               //new
    void logOut();
    void switchApps();
    void openApp();
    void updateTime();                  //new
    void closeEvent(QCloseEvent *event);
    void openCalendarAction();
    void openClockAction();

private slots:
    void openNotepad(QString path);
};
#endif // MYAPP_H
