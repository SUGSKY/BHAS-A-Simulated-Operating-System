#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include<QGridLayout>
#include<QLabel>
#include<QMainWindow>
#include<QMap>
#include<QProgressBar>
#include<QPushButton>
#include<QSpacerItem>
#include<QVBoxLayout>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class taskManager; }
QT_END_NAMESPACE

class taskManager : public QMainWindow
{
    Q_OBJECT

public:
    taskManager(QWidget *parent = nullptr);
    ~taskManager();
    //add below

signals:
    void openCalendar();
    void openClock();

private:
    Ui::taskManager *ui;
    QAction *dateAction;
    QAction *timeAction;
    QMenu *app_list;
    QProgressBar *CPULevel;
    QProgressBar *RAMLevel;
    QProgressBar *DISKLevel;
    QGridLayout *taskListLayout;
    QVBoxLayout *graphsCPULevelLayout;
    QVBoxLayout *graphsRAMLevelLayout;
    QVBoxLayout *graphsDISKLevelLayout;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    QVector<QLabel*> currTask;
    QVector<QLabel*> currTaskCPU;
    QVector<QLabel*> currTaskRAM;
    QVector<QLabel*> currTaskDISK;
    QVector<QPushButton*> endTaskButton;
    QWidget *taskListWidget;
    QWidget *graphsCPULevelWidget;
    QWidget *graphsRAMLevelWidget;
    QWidget *graphsDISKLevelWidget;
    void appListUpdate();
    void switchApps();
    void close_app();
    void updateStatus();
    void updateTime();
    void endTask();
    void closeEvent(QCloseEvent *event);


};
#endif // TASKMANAGER_H
