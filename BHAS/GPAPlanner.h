#ifndef GPAPLANNER_H
#define GPAPLANNER_H

#include <QMainWindow>
#include <QMap>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GPAPlanner; }
QT_END_NAMESPACE

class GPAPlanner : public QMainWindow
{
    Q_OBJECT

public:
    GPAPlanner(QWidget *parent = nullptr);
    ~GPAPlanner();
    //add below

signals:
    void openCalendar();
    void openClock();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GPAPlanner *ui;
    QMenu *app_list;
    QAction *dateAction;
    QAction *timeAction;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void switchApps();
    void close_app();
    void updateTime();                  //new
    //add below


};
#endif // GPAPLANNER_H
