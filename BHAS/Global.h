#ifndef GLOBAL_H
#define GLOBAL_H

#include<QMap>
#include<QWidget>

extern QList<QString> appname;
extern QMap<QString, QWidget*> openedApps;
extern QMap<QString, QVector<int>> appUsage;


#endif // GLOBAL_H
