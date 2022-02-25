#ifndef FOLDERBUTTON_H
#define FOLDERBUTTON_H

#include<QMouseEvent>
#include<QDebug>
#include<QIcon>
#include<QLabel>
#include<QObject>
#include<QPushButton>
#include<QString>
#include<QTime>
#include<QWidget>
#include<QVector>

class folderButton :public QWidget{
    Q_OBJECT
public:
    folderButton(QWidget *parent);
    bool isChecked();
    QString path();
    QString getName();
    QString getType();
    void setPath(QString str);
    void setType(QString str);

signals:
    void selected(bool sel);
    void fileOpened();


public slots:
    void renameFolder(QString str);
    void setFileIcon(QIcon fileIcon);

private:
    QPushButton *button;
    QLabel *name;
    QString folder_path;
    QString folder_name;
    QString type;
    QTime interval;

private slots:
    void select(bool val);
};

#endif // FOLDERBUTTON_H
