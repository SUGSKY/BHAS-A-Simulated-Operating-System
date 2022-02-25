#include<QApplication>
#include<QLayout>
#include<QPixmap>
#include<QSize>
#include "folderbutton.h"

folderButton::folderButton(QWidget *parent):QWidget(parent){
    //Variables
    QIcon folder_icon(":/img/folder-icon.png");
    type = "FOLDER";

    //Widgets Declare
    button = new QPushButton(this);
    name = new QLabel(this);
    this->setFixedSize(120,120);

    //Widgets Setup
    button->setText("");
    button->setFlat(1);
    button->setCheckable(1);
    button->setGeometry(0, 0, 100, 60);
    button->setIcon(folder_icon);
    button->setIconSize(QSize(100,60));
    button->setAutoDefault(1);

    renameFolder("New Folder");

    connect(button, SIGNAL(toggled(bool)), this, SLOT(select(bool)));
}

bool folderButton::isChecked(){
    return button->isChecked();
}

QString folderButton::path(){
    return folder_path;
}

void folderButton::setPath(QString str){
    folder_path = str;
}

void folderButton::renameFolder(QString str){
    folder_name = str;
    this->setObjectName(folder_name);
    QString label;
    if(folder_name.length()>12){
        if(folder_name.length()>36){
            for(int i = 0; i<34; i++){
                if(i%12==0 && i!=0) label+='\n';
                label+=folder_name[i];
            }
            label+="..";
        }else for(int i = 0; i<folder_name.length(); i++){
            if(i%12==0 && i!=0) label+='\n';
            label+=folder_name[i];
        }
    }else label = folder_name;
    name->setText(folder_name);
    name->setText(label);
    name->setAlignment(Qt::AlignCenter);
    name->setGeometry(0, 55, name->width(), 20*(label.length()/12+1));
}

void folderButton::select(bool val){
    emit selected(val);
    QTime now = QTime::currentTime();
    if(now.msecsSinceStartOfDay()-interval.msecsSinceStartOfDay() < 500){
        emit fileOpened();
    }
    interval = now;
}

void folderButton::setFileIcon(QIcon fileIcon){
    button->setIcon(fileIcon);
}

QString folderButton::getName(){
    return folder_name;
}

QString folderButton::getType(){
    return type;
}

void folderButton::setType(QString str){
    type = str;
}
