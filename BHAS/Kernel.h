#ifndef KERNEL_H
#define KERNEL_H

#include <QMainWindow>
#include <QMap>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Kernel; }
QT_END_NAMESPACE

class Kernel : public QMainWindow
{
    Q_OBJECT

public:
    Kernel(QWidget *parent = nullptr);
    ~Kernel();
signals:
    void sendMessage(const QString &msg);
    void openCalendar();
    void openClock();
private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::Kernel *ui;
    QMenu *app_list;
    QAction *dateAction;
    QAction *timeAction;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void updateTime();
    void switchApps();
    void close_app();
};
#endif // KERNEL_H
