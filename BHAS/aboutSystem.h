#ifndef ABOUTSYSTEM_H
#define ABOUTSYSTEM_H

#include <QMainWindow>
#include <QMap>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class aboutSystem; }
QT_END_NAMESPACE

class aboutSystem : public QMainWindow
{
    Q_OBJECT

public:
    aboutSystem(QWidget *parent = nullptr);
    ~aboutSystem();
    //add below

signals:
    void openCalendar();
    void openClock();

private:
    Ui::aboutSystem *ui;
    QAction *dateAction;
    QAction *timeAction;
    QMenu *app_list;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void switchApps();
    void close_app();
    void updateTime();
    //add below


};
#endif // ABOUTSYSTEM_H
