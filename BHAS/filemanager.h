#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include<QStringList>
#include<QFile>
#include<QFileInfo>
#include<QGridLayout>
#include <QMainWindow>
#include<QMap>
#include"folderbutton.h"
#include"Global.h"

namespace Ui {
class FileManager;
}

class FileManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileManager(QWidget *parent = nullptr);
    ~FileManager();

signals:
    void openCalendar();
    void openClock();
    void openNotepad(QString path);

private slots:
    void toggle_checkedFolder(bool val);

private:
    Ui::FileManager *ui;
    bool moveCheck;

    folderButton* folder_temp;

    QAction *dateAction;
    QAction *timeAction;
    QFileInfoList currentFiles;
    QGridLayout *file_layout;
    QLabel *pathLabel;
    QMenu *app_list;
    QVector<folderButton*> folders;
    QVector<QAction*> appFeatures;
    QVector<QAction*> appList;
    QWidget *file_space;
    void updateTime();
    void appListUpdate();
    void appName_list();
    void preDir();
    void generateFiles(QString target_path);
    bool copyAllFiles(QString src, QString dest);
    void newFolder();
    void copyFolders();
    void moveFolders();
    void pasteFolders();
    void renameFolder();
    void deleteFolders();
    void close_app();
    void switchApps();
    void openFile();
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *event);
};
#endif // FILEMANAGER_H
