#include<QDateTime>
#include<QInputDialog>
#include<QTimer>
#include "Notepad.h"
#include "ui_Notepad.h"

//Global Variables (global variables for this app only)
const QString program_name = "Notepad";

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(6);
    currAppUsage.append(10);
    currAppUsage.append(11);
    appUsage[program_name] = currAppUsage;

//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    savePath = "";
    //start below



//Widgets (initialization for widget)
    //start below
    appFeatures.append(new QAction("New", this));
    appFeatures.append(new QAction("Save", this));
    appFeatures.append(new QAction("Save As", this));
    appFeatures.append(new QAction("Close", this));

    appEdit.append(new QAction("Copy", this));
    appEdit.append(new QAction("Paste", this));
    appEdit.append(new QAction("Cut", this));
    appEdit.append(new QAction("Undo", this));
    appEdit.append(new QAction("Redo", this));

    QTimer *updater = new QTimer(this);

//Widgets setup (set the widget properties)
    //start below
    appFeatures[0]->setStatusTip("New text file");
    appFeatures[1]->setStatusTip("Save change(s)");
    appFeatures[2]->setStatusTip("Save this text file");
    appFeatures[3]->setStatusTip("Close application");

    appEdit[0]->setStatusTip("Copy text");
    appEdit[1]->setStatusTip("Paste text");
    appEdit[2]->setStatusTip("Cut text");
    appEdit[3]->setStatusTip("Undo change(s)");
    appEdit[4]->setStatusTip("Redo change(s)");

    QMenuBar *dateTime = new QMenuBar(ui->menubar);
    dateAction = new QAction(QDateTime::currentDateTime().date().toString(), this);
    timeAction = new QAction(QDateTime::currentDateTime().time().toString(), this);
    dateTime->addAction(dateAction);
    dateTime->addAction(timeAction);
    ui->menubar->setCornerWidget(dateTime, Qt::TopRightCorner);

    app_list = menuBar()->addMenu("");
    app_list->setIcon(applist_icon);
    appname = openedApps.keys();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    QMenu *app_name = menuBar()->addMenu(program_name);
    //here add app features (e.g. copy, close, etc)
    app_name->addAction(appFeatures[0]);
    app_name->addAction(appFeatures[1]);
    app_name->addAction(appFeatures[2]);
    app_name->addAction(appFeatures[3]);

    QMenu *app_edit = menuBar()->addMenu("Edit");
    app_edit->addAction(appEdit[0]);
    app_edit->addAction(appEdit[1]);
    app_edit->addAction(appEdit[2]);
    app_edit->addAction(appEdit[3]);
    app_edit->addAction(appEdit[4]);




    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){                                        //don't delete
        connect(appList[i], &QAction::triggered, this, &Notepad::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &Notepad::onactionNewtriggered);
    connect(appFeatures[1], &QAction::triggered, this, &Notepad::onactionSavetriggered);
    connect(appFeatures[2], &QAction::triggered, this, &Notepad::onactionSaveastriggered);
    connect(appFeatures[3], &QAction::triggered, this, &Notepad::close_app);

    connect(appEdit[0], &QAction::triggered, this, &Notepad::onactionCopytriggered);
    connect(appEdit[1], &QAction::triggered, this, &Notepad::onactionPastetriggered);
    connect(appEdit[2], &QAction::triggered, this, &Notepad::onactionCuttriggered);
    connect(appEdit[3], &QAction::triggered, this, &Notepad::onactionUndotriggered);
    connect(appEdit[4], &QAction::triggered, this, &Notepad::onactionRedotriggered);

    connect(app_list, &QMenu::aboutToShow, this, &Notepad::appListUpdate);
    connect(updater, &QTimer::timeout, this, &Notepad::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));


}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &Notepad::switchApps);
    }
}

void Notepad::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void Notepad::onactionNewtriggered()
{
    // Global referencing the current file that we are clearing
    currentFile.clear();

    // Clear the textEdit widget buffer
    ui->textEdit->setText(QString());
}

void Notepad::onactionSaveastriggered()
{
    // Opens a dialog for saving a file
//    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QString path = QInputDialog::getText(this, "Save as", "Save Path: (use 'OS" + QString(QDir::separator()) + "')", QLineEdit::Normal, "OS" + QString(QDir::separator()), nullptr, Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    QString fileName = QDir::currentPath() + QString(QDir::separator()) + "DATA" + QString(QDir::separator()) + path.right(path.size()-3);

    if(path == "" || path == "OS" + QString(QDir::separator())){
        QMessageBox::warning(this, "Save as", "Cannot save file with blank name!");
        return;
    }

    // An object for reading and writing files
    QFile file(fileName);
    if(file.exists()){
        if(QMessageBox::question(this, "Save as", "Another text file with the same name already exists. Do you want to overwrite that file?") == QMessageBox::No) return;
    }

    // Try to open a file with write only options
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Save as", "Cannot save file: " + file.errorString());
        return;
    }

    // Store the currentFile name
    currentFile = fileName;

    // Set the title for the window to the file name
    setWindowTitle(fileName);

    // Interface for writing text
    QTextStream out(&file);

    // Copy text in the textEdit widget and convert to plain text
    QString text = ui->textEdit->toPlainText();

    // Output to file
    out << text;

    // Close the file
    file.close();

    savePath = fileName;
}

void Notepad::onactionSavetriggered()
{
    if(savePath == ""){
        onactionSaveastriggered();
        return;
    }
    QString fileName = savePath;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void Notepad::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    close();
}


void Notepad::onactionCopytriggered()
{
    ui->textEdit->copy();
}

void Notepad::onactionCuttriggered()
{
    ui->textEdit->cut();
}

void Notepad::onactionPastetriggered()
{
    ui->textEdit->paste();
}

void Notepad::onactionUndotriggered()
{
    ui->textEdit->undo();
}

void Notepad::onactionRedotriggered()
{
    ui->textEdit->redo();
}

void Notepad::openFromOutside(QString path){
    savePath = path;
    QFile file(path);
    currentFile = path;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(path);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}
