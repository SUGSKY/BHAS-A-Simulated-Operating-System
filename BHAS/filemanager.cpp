#include<QDateTime>
#include<QDebug>
#include<QDir>
#include<QIcon>
#include<QInputDialog>
#include<QLayoutItem>
#include<QLineEdit>
#include<QMessageBox>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include<QTimer>
#include<QVector>
#include<QScreen>
#include<string>
#include "filemanager.h"
#include "ui_filemanager.h"

//Global Variables
const QString baseFolder = QDir::currentPath();
const QString program_name = "File Manager";
const QStringList supportedFiles = {"*.txt"};
int file_column = 8;
int file_row = 5;
int fileCount = 0;
bool moveCheck = 0;
QVector<folderButton*> checkedFolder;
QVector<QStringList> copiedFolder;

FileManager::FileManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileManager)
{
    ui->setupUi(this);

    //Setup
    file_column = (qApp->screens()[0]->size().width()-50)/128;
    file_row = (qApp->screens()[0]->size().height()-150)/120;
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(10);
    currAppUsage.append(11);
    currAppUsage.append(15);
    appUsage[program_name] = currAppUsage;

    QIcon applist_icon(":/img/a.png");
    QPalette pal_white = palette();
    pal_white.setColor(QPalette::Background, Qt::white);
    QPalette pal_lightGray = palette();
    pal_white.setColor(QPalette::Background, Qt::lightGray);
    QSizePolicy size_horizontal, size_vertical;
    size_horizontal.setHorizontalStretch(1);
    size_vertical.setVerticalStretch(1);

    //Widgets
    appFeatures.append(new QAction("New", this));
    appFeatures.append(new QAction("Copy", this));
    appFeatures.append(new QAction("Move", this));
    appFeatures.append(new QAction("Paste", this));
    appFeatures.append(new QAction("Rename", this));
    appFeatures.append(new QAction("Delete", this));
    appFeatures.append(new QAction("Close", this));

    QTimer *updater = new QTimer(this);

    QFrame *line = new QFrame;
    QPushButton *Back = new QPushButton;
    pathLabel = new QLabel;

    //Widgets setup
    appFeatures[0]->setStatusTip("Make a new folder");
    appFeatures[1]->setStatusTip("Copy file(s)");
    appFeatures[2]->setStatusTip("Move file(s)");
    appFeatures[3]->setStatusTip("Paste file(s)");
    appFeatures[4]->setStatusTip("Rename a file");
    appFeatures[5]->setStatusTip("Delete file(s)");
    appFeatures[6]->setStatusTip("Close application");

    app_list = menuBar()->addMenu("");
    app_list->setIcon(applist_icon);
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }

    QMenu *app_name = menuBar()->addMenu(program_name);
    app_name->addAction(appFeatures[0]);
    app_name->addAction(appFeatures[1]);
    app_name->addAction(appFeatures[2]);
    app_name->addAction(appFeatures[3]);
    app_name->addAction(appFeatures[4]);
    app_name->addAction(appFeatures[5]);
    app_name->addAction(appFeatures[6]);

    QMenuBar *dateTime = new QMenuBar(ui->menubar);
    dateAction = new QAction(QDateTime::currentDateTime().date().toString(), this);
    timeAction = new QAction(QDateTime::currentDateTime().time().toString(), this);
    dateTime->addAction(dateAction);
    dateTime->addAction(timeAction);
    ui->menubar->setCornerWidget(dateTime, Qt::TopRightCorner);

    Back->setText("< Back");
    Back->setFixedSize(90,35);
    Back->setAutoDefault(1);

    pathLabel->setFixedHeight(35);

    line->setFrameShape(QFrame::HLine);

    //Layouts
    QWidget *toolbar_widget = new QWidget;
    QHBoxLayout *toolbar_layout = new QHBoxLayout(toolbar_widget);
    toolbar_widget->setAutoFillBackground(1);
    toolbar_widget->setPalette(pal_white);
    toolbar_layout->addWidget(Back);
    toolbar_layout->addWidget(pathLabel);
    toolbar_layout->addStretch(1);
    toolbar_widget->setFixedHeight(60);

    file_space = new QWidget;
    file_layout = new QGridLayout(file_space);
    file_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    file_space->setFixedSize(qApp->screens()[0]->size().width()-50, qApp->screens()[0]->size().height()-150);
    generateFiles("");

    ui->main_layout->addWidget(toolbar_widget);
    ui->main_layout->addWidget(line);
    ui->main_layout->addWidget(file_space);
    ui->main_layout->addStretch(1);

    //Connection
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &FileManager::switchApps);
    }
    connect(app_list, &QMenu::aboutToShow, this, &FileManager::appListUpdate);
    connect(app_name, &QMenu::aboutToShow, this, &FileManager::appName_list);
    connect(updater, &QTimer::timeout, this, &FileManager::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    connect(Back, &QPushButton::clicked, this, &FileManager::preDir);

    connect(appFeatures[0], &QAction::triggered, this, &FileManager::newFolder);
    connect(appFeatures[1], &QAction::triggered, this, &FileManager::copyFolders);
    connect(appFeatures[2], &QAction::triggered, this, &FileManager::moveFolders);
    connect(appFeatures[3], &QAction::triggered, this, &FileManager::pasteFolders);
    connect(appFeatures[4], &QAction::triggered, this, &FileManager::renameFolder);
    connect(appFeatures[5], &QAction::triggered, this, &FileManager::deleteFolders);
    connect(appFeatures[6], &QAction::triggered, this, &FileManager::close_app);
}

FileManager::~FileManager()
{
    delete ui;
}

void FileManager::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &FileManager::switchApps);
    }
}

void FileManager::appName_list(){
    if(checkedFolder.size() == 0){
        appFeatures[1]->setEnabled(0);
        appFeatures[2]->setEnabled(0);
        appFeatures[4]->setEnabled(0);
        appFeatures[5]->setEnabled(0);
    }else if(checkedFolder.size() == 1){
        appFeatures[1]->setEnabled(1);
        appFeatures[2]->setEnabled(1);
        appFeatures[4]->setEnabled(1);
        appFeatures[5]->setEnabled(1);
    }else{
        appFeatures[1]->setEnabled(1);
        appFeatures[2]->setEnabled(1);
        appFeatures[4]->setEnabled(0);
        appFeatures[5]->setEnabled(1);
    }
    if(copiedFolder.size() == 0) appFeatures[3]->setEnabled(0);
    else appFeatures[3]->setEnabled(1);
}

void FileManager::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void FileManager::toggle_checkedFolder(bool val){
    folderButton *folder = (folderButton*) sender();
    if(val){
        checkedFolder.append(folder);
    }else{
        checkedFolder.removeOne(folder);
    }
}

void FileManager::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

bool FileManager::copyAllFiles(QString src, QString dest){
    bool exist;
    QFileInfo folder;
    QDir src_path(src), dest_path(dest);
    folder.setFile(src);
    if(folder.isFile()){
        exist = QFile::copy(folder.filePath(), (dest_path.path() + QDir::separator() + folder.fileName()));
        if(!exist) QMessageBox::warning(this, "Copying File(s)", "Unable to copy '" + folder.fileName() + "'");
        return exist;
    }else{
        exist = dest_path.mkdir(folder.fileName());
    }
    if(!exist){
        QMessageBox::warning(this, "Copying File(s)", "Unable to copy '" + folder.fileName() + "'");
        return exist;
    }
    foreach (QFileInfo file, src_path.entryInfoList(supportedFiles, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files)){
        exist &= copyAllFiles(file.path() + QString(QDir::separator()) + file.fileName(), dest_path.path() + QString(QDir::separator()) + folder.fileName());
    }
    return exist;
}

void FileManager::newFolder(){
    bool exist = 0;
    int counter = 1;
    QString newName = "NewFolder";
    while(1){
        exist = 0;
        counter++;
        for(int i = 0; i<currentFiles.size(); i++){
            if(currentFiles[i].fileName().toLower() == newName.toLower()){
                exist = 1;
                break;
            }
        }
        if(!exist) break;
        newName = "NewFolder(" + QString::number(counter) + ")";
    }
    QDir curDir(baseFolder + QDir::separator() + "DATA" + QDir::separator() + pathLabel->text().right(pathLabel->text().size()-3));
    exist = curDir.mkdir(newName);
    if(!exist){
        QMessageBox::warning(this, "Making new folder", "Unable to make a new folder");
        return;
    }
    generateFiles(pathLabel->text().right(pathLabel->text().size()-3));
}

void FileManager::copyFolders(){
    copiedFolder.clear();
    QStringList temp;
    for(int i = 0; i<checkedFolder.size(); i++){
        temp.clear();
        temp.append(checkedFolder[i]->getName());
        temp.append(checkedFolder[i]->path());
        copiedFolder.append(temp);
    }
    moveCheck = 0;
}

void FileManager::moveFolders(){
    copiedFolder.clear();
    QStringList temp;
    for(int i = 0; i<checkedFolder.size(); i++){
        temp.clear();
        temp.append(checkedFolder[i]->getName());
        temp.append(checkedFolder[i]->path());
        copiedFolder.append(temp);
    }
    moveCheck = 1;
}

void FileManager::pasteFolders(){
    bool succeed;
    QString srcPath, destPath, temp;
    destPath = baseFolder + QDir::separator() + "DATA" + QDir::separator() + pathLabel->text().right(pathLabel->text().size()-3);
    temp = pathLabel->text().right(pathLabel->text().size()-3);
    for(int i = 0; i<copiedFolder.size(); i++) {
        srcPath = baseFolder + QDir::separator() + "DATA" + QDir::separator() + copiedFolder[i][1] + QString(QDir::separator()) + copiedFolder[i][0];
        QFileInfo src(srcPath), dest(destPath);
        if(dest.absoluteFilePath().startsWith(src.absoluteFilePath())){
            QMessageBox::warning(this, "Copying Folder '" + src.fileName() + "'", "The destination folder is a subfolder of the source folder");
            continue;
        }
        succeed = copyAllFiles(srcPath, destPath);
        if(moveCheck && succeed){
            checkedFolder.clear();
            checkedFolder.append(new folderButton(this));
            checkedFolder[0]->renameFolder(copiedFolder[i][0]);
            checkedFolder[0]->setPath(copiedFolder[i][1]);
            deleteFolders();
        }
        generateFiles(temp);
    }
}

void FileManager::renameFolder(){
    bool temp = 0;
    folderButton *folder = checkedFolder[0];
    QString input = QInputDialog::getText(this, "Rename File/Folder", "Rename File/Folder:", QLineEdit::Normal, QString(), nullptr, Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    for(int i = 0; i<fileCount; i++) if(folders[i]->getName().toLower() == input.toLower()){
        temp = 1;
        break;
    }
    if(folder->getType() == "TXT" && (!input.endsWith(".txt"))){
        QMessageBox::warning(this, "Renaming File/Folder", "Unable to change file extension!");
        return;
    }
    if(!temp){
        QDir sysFolder;
        sysFolder.setPath(baseFolder + QDir::separator() + "DATA"+ QDir::separator() + folder->path());
        temp = sysFolder.rename(folder->getName(), input);
        if(temp){
            folder->renameFolder(input);
            generateFiles(folder->path());
        }else{
            QMessageBox::warning(this, "Renaming File/Folder", "Unable to rename File/folder '" + folder->getName() + "' to '" + input + "'");
        }
    }else{
        QMessageBox::warning(this, "Renaming File/Folder", "This destination already contain a file/folder named '" + input + "'");
    }
}

void FileManager::deleteFolders(){
    QDir file;
    QFileInfo file_info;
    QString path;

    path = checkedFolder[0]->path();
    for(int i = 0; i<checkedFolder.size(); i++){
        file.setPath(baseFolder + QString(QDir::separator()) + "DATA" + QString(QDir::separator()) + path);
        file_info.setFile(file, checkedFolder[i]->getName());
        if(file_info.isDir()){
            file.setPath(file.path() + QString(QDir::separator()) + file_info.fileName());
            file.removeRecursively();
        }else if(file_info.isFile()){
            QFile tempFile(file_info.filePath());
            if(!tempFile.remove()) QMessageBox::warning(this, "Deleting File", "Unable to delete file '" + checkedFolder[i]->getName() + "'");
        }
    }
    generateFiles(path);
}

void FileManager::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    close();
}

void FileManager::generateFiles(QString target_path){
    QDir realPath;
    if(target_path == ""){
        realPath.setPath(baseFolder+QDir::separator()+"DATA");
        if(!realPath.exists()){
            realPath.mkpath(".");
        }
    }else{
        realPath.setPath(baseFolder + QDir::separator() + "DATA" + QDir::separator() + target_path);
    }
    for(int i = 0; i<folders.size(); i++) folders[i]->hide();
    folders.clear();
    currentFiles = realPath.entryInfoList(supportedFiles, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files, QDir::DirsFirst | QDir::Name);
    fileCount = currentFiles.size();
    for(int i = 0; i<fileCount; i++){
        folders.append(new folderButton(this));
        folders[i]->renameFolder(currentFiles[i].fileName());
        folders[i]->setPath(target_path);
        if(currentFiles[i].isFile() && currentFiles[i].fileName().endsWith(".txt", Qt::CaseInsensitive)){
            QIcon textFile_icon(":/img/TXT.png");
            folders[i]->setType("TXT");
            folders[i]->setFileIcon(textFile_icon);
        }
        file_layout->addWidget(folders[i], i/file_column, i%file_column, Qt::AlignTop | Qt::AlignLeft);
        connect(folders[i], SIGNAL(selected(bool)), this, SLOT(toggle_checkedFolder(bool)));
        connect(folders[i], &folderButton::fileOpened, this, &FileManager::openFile);
    }
    pathLabel->setText("OS" + QString(QDir::separator()) + target_path);
    checkedFolder.clear();
}

void FileManager::openFile(){                           //this
    folderButton *folder = (folderButton*) sender();
    QString target;
    target = folder->path();
    if(target.size()!=0) target += QDir::separator();
    target += folder->getName();
    if(folder->getType() == "FOLDER"){
        generateFiles(target);
    }else if(folder->getType() == "TXT"){
        emit openNotepad(baseFolder + QDir::separator() + "DATA" + QDir::separator() + target);
    }
}

void FileManager::preDir(){
    QString curPath = pathLabel->text();
    curPath = curPath.right(curPath.size()-3);
    if(curPath.size()!=0){
        int i = 0;
        while(curPath.indexOf(QDir::separator(), i+1) != -1){
            i = curPath.indexOf(QDir::separator(), i+1);
        }
        curPath = curPath.left(i);
        generateFiles(curPath);
    }
}

void FileManager::showEvent(QShowEvent *){
    generateFiles(pathLabel->text().right(pathLabel->text().size()-3));
}

void FileManager::closeEvent(QCloseEvent *event){
    close_app();
}
