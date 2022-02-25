#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <QMap>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
    //add below

signals:
    void openCalendar();
    void openClock();

private:
    Ui::Calculator *ui;
    QMenu *app_list;
    QAction *dateAction;
    QAction *timeAction;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void switchApps();
    void close_app();
    void updateTime();
    //add below

private slots :
    void NumPressed();
    void MathButtonPressed();
    void EqualButtonPressed();
    void ChangeNumberSign();
    void on_Clear_clicked();

};
#endif // CALCULATOR_H
