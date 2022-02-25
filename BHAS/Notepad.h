#ifndef Notepad_H
#define Notepad_H

// Provides the main application window on which the user interface
// is built by adding widgets
#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include"Global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    void openFromOutside(QString path);
    ~Notepad();

signals:
    void openCalendar();
    void openClock();

private slots:
    void onactionNewtriggered();

    void onactionSaveastriggered();

    void onactionSavetriggered();

    void onactionCopytriggered();

    void onactionCuttriggered();

    void onactionPastetriggered();

    void onactionUndotriggered();

    void onactionRedotriggered();

private:
    Ui::Notepad *ui;
    QMenu *app_list;
    QAction *dateAction;
    QAction *timeAction;
    QString savePath;
    QVector<QAction*> appEdit;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    void appListUpdate();
    void close_app();
    void switchApps();
    void updateTime();

    QString currentFile = "";
};
#endif // Notepad_H
