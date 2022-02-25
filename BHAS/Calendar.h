#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include <QMap>
#include <QWidget>
#include <QLineEdit>
#include <QTimeEdit>
#include <QTextEdit>
#include <QPushButton>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Calendar; }
QT_END_NAMESPACE

class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    Calendar(QWidget *parent = nullptr);
    ~Calendar();
    //add below

signals:
    void openClock();

private:
    Ui::Calendar *ui;
    QMenu *app_list;
    QAction *dateAction;
    QAction *timeAction;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void switchApps();
    void close_app();
    void updateTime();
    void clearForm();
    void saveEvent();
    void showEvent();
    void deleteEvent();
    void checkDeleteButton();
    void showDetails();
    void clearDetails();
    //add below


};
#endif // CALENDAR_H
