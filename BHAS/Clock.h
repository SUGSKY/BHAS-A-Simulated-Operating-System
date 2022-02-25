#ifndef CLOCK_H
#define CLOCK_H

#include <QMainWindow>
#include <QMap>
#include <QLCDNumber>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Clock; }
QT_END_NAMESPACE

class Clock : public QMainWindow
{
    Q_OBJECT

public:
    Clock(QWidget *parent = nullptr);
    ~Clock();
    //add below

signals:
    void openCalendar();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::Clock *ui;QMenu *app_list;
    QAction *dateAction;
    QAction *timeAction;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void switchApps();
    void updateTime();
    void close_app();
    //add below
    void stopwatchStart();
    void stopwatchPause();
    void stopwatchReset();
    void updateCountdown();
    void timerStart();
    void timerPause();
    void timerReset();
    void updateTimer();
};

class analogClock : public QWidget
{
    Q_OBJECT

public:
    analogClock(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void showTime();
};

class digitalClock : public QLCDNumber
{
    Q_OBJECT

public:
    digitalClock(QWidget *parent = 0);
private slots:
    void showTime();
};






#endif // CLOCK_H
