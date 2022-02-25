#include<QDateTime>
#include<QDebug>
#include<QIcon>
#include<QPalette>
#include<QPixmap>
#include<QSize>
#include<QTimer>
#include<QVector>
#include "GPAPlanner.h"
#include "ui_GPAPlanner.h"



//Global Variables (global variables for this app only)
const QString program_name = "GPA Planner";             //Change "Desktop" to your app name
//start below
//added by brand
#include <vector>
#include <map>
#include <QMessageBox>
#include <cstdlib>
using namespace std;

GPAPlanner::GPAPlanner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GPAPlanner)
{
    ui->setupUi(this);
    openedApps[program_name] = this;
    appname = openedApps.keys();
    QVector<int> currAppUsage;
    currAppUsage.append(13);
    currAppUsage.append(10);
    currAppUsage.append(7);
    appUsage[program_name] = currAppUsage;

//Setup (initialization for picture, color, etc)
    QIcon applist_icon(":/img/a.png");
    //start below



//Widgets (initialization for widget)
    //start below
    appFeatures.append(new QAction("Close", this));                 //add features (please modify)

    QTimer *updater = new QTimer(this);



//Widgets setup (set the widget properties)
    //start below
    appFeatures[0]->setStatusTip("Close application");              //set explaination for feature 0 (please modify)

    QMenuBar *dateTime = new QMenuBar(ui->menubar);
    dateAction = new QAction(QDateTime::currentDateTime().date().toString(), this);
    timeAction = new QAction(QDateTime::currentDateTime().time().toString(), this);
    dateTime->addAction(dateAction);
    dateTime->addAction(timeAction);
    ui->menubar->setCornerWidget(dateTime, Qt::TopRightCorner);

    app_list = menuBar()->addMenu("");
    app_list->setIcon(applist_icon);
    appname = openedApps.keys();                                  //don't delete
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    QMenu *app_name = menuBar()->addMenu(program_name);
    //here add app features (e.g. copy, close, etc)
    app_name->addAction(appFeatures[0]);                            //add feature 0 to layout (please modify)



//Layouts (set the display layout)
    QWidget *file_space = new QWidget;
    QGridLayout *file_layout = new QGridLayout(file_space);
    file_layout->setAlignment(Qt::AlignLeft);
    //here add app name                                             //main content of your app



    ui->main_layout->addWidget(file_space);
    ui->main_layout->addStretch(1);
    //don't add code here

//Connection (set signal and slot)
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &GPAPlanner::switchApps);
    }
    connect(appFeatures[0], &QAction::triggered, this, &GPAPlanner::close_app);
    connect(app_list, &QMenu::aboutToShow, this, &GPAPlanner::appListUpdate);
    connect(updater, &QTimer::timeout, this, &GPAPlanner::updateTime);
    updater->start(1000);
    connect(dateAction, SIGNAL(triggered()), this, SIGNAL(openCalendar()));
    connect(timeAction, SIGNAL(triggered()), this, SIGNAL(openClock()));
    //start below
    QMessageBox::information(this, "Welcome to GPA Planner", "Hi, Welcome! Please spend some time to read this. In the middle of the term, you can usually estimate some of your grades (be they good or bad). But, some are still not obvious. This app will show you all the combinations to achieve your target gpa.");
        QStringList list=(QStringList()<<"A"<<"A-"<<"B+"<<"Plan"<<"B"<<"B-"<<"C+"<<"C"<<"C-"<<"D"<<"F");
        ui->gpa1->addItems(list);
        ui->gpa2->addItems(list);
        ui->gpa3->addItems(list);
        ui->gpa4->addItems(list);
        ui->gpa5->addItems(list);
        ui->gpa6->addItems(list);
        ui->gpa7->addItems(list);
        ui->gpa8->addItems(list);
        ui->textEdit->setReadOnly(true);

}

GPAPlanner::~GPAPlanner()
{
    delete ui;
}

void GPAPlanner::appListUpdate(){
    appList.clear();
    app_list->clear();
    for(int i = 0; i<appname.size(); i++){
        appList.append(new QAction(appname[i], this));
        app_list->addAction(appList[i]);
    }
    for(int i = 0; i<appname.size(); i++){
        connect(appList[i], &QAction::triggered, this, &GPAPlanner::switchApps);
    }
}

void GPAPlanner::switchApps(){
    QAction *app = (QAction*) sender();

    if(openedApps[app->text()] != this){
        openedApps[app->text()]->show();
        this->hide();
    }
}

void GPAPlanner::updateTime(){
    dateAction->setText(QDateTime::currentDateTime().date().toString());
    timeAction->setText(QDateTime::currentDateTime().time().toString());
}

void GPAPlanner::close_app(){
    appname.removeOne(program_name);
    openedApps.remove(program_name);
    openedApps["Desktop"]->show();
    this->close();
}

//add function below

string is_double(const std::string& s)
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    if ( end != s.c_str() && *end == '\0' && val != 999999 )
        return "true";
    else {
        return "false";
    }
}
string is_integer(string s) {
    if (is_double(s) == "true") {
        if (s.find(".") == -1) {
            return "true";
        }
        else {
            return "false";
        }
    }
    return "false";
}
void GPAPlanner::on_pushButton_2_clicked()
{
    ui->textEdit->setText("");
        int maxlength = 8;
        int totalCredit = 0;
        vector<string> confusedCreditList;
        double totalgpa = 0;
        map<string,double> possible;
        possible["A"] = 4.0;
        possible["A-"] = 3.7;
        possible["B+"] = 3.3;
        possible["B"] = 3.0;
        possible["B-"] = 2.7;
        possible["C+"] = 2.3;
        possible["C"] = 2.0;
        possible["C-"] = 1.7;
        possible["D+"] = 1.3;
        possible["D"] = 1.0;
        possible["F"] = 0.0;
        possible["Plan"] = -1.0;
        map<double,string> possibleR;
        possibleR[4.0] = "A";
        possibleR[3.7] = "A-";
        possibleR[3.3] = "B+";
        possibleR[3.0] = "B";
        possibleR[2.7] = "B-";
        possibleR[2.3] = "C+";
        possibleR[2.0] = "C";
        possibleR[1.7] = "C-";
        possibleR[1.3] = "D+";
        possibleR[1.0] = "D";
        possibleR[0.0] = "F";
        possibleR[-1.0] = "Plan";
        vector<double> possibleList = {0.0, 1.0, 1.3, 1.7, 2.0, 2.3, 2.7, 3.0, 3.3, 3.7, 4.0};
        int possibleLength = 11;
        string flag = "true";
        string flag2 = "true";


        string targetgpa = ui->targetgpa->text().toStdString();

        string course1 = ui->course1->text().toStdString();
        string credit1 = ui->credit1->text().toStdString();
        string gpa1 = ui->gpa1->currentText().toStdString();

        string course2 = ui->course2->text().toStdString();
        string credit2 = ui->credit2->text().toStdString();
        string gpa2 = ui->gpa2->currentText().toStdString();

        string course3 = ui->course3->text().toStdString();
        string credit3 = ui->credit3->text().toStdString();
        string gpa3 = ui->gpa3->currentText().toStdString();

        string course4 = ui->course4->text().toStdString();
        string credit4 = ui->credit4->text().toStdString();
        string gpa4 = ui->gpa4->currentText().toStdString();

        string course5 = ui->course5->text().toStdString();
        string credit5 = ui->credit5->text().toStdString();
        string gpa5 = ui->gpa5->currentText().toStdString();

        string course6 = ui->course6->text().toStdString();
        string credit6 = ui->credit6->text().toStdString();
        string gpa6 = ui->gpa6->currentText().toStdString();

        string course7 = ui->course7->text().toStdString();
        string credit7 = ui->credit7->text().toStdString();
        string gpa7 = ui->gpa7->currentText().toStdString();

        string course8 = ui->course8->text().toStdString();
        string credit8 = ui->credit8->text().toStdString();
        string gpa8 = ui->gpa8->currentText().toStdString();


        if (credit1 != "") {
            if (course1 != "") {

            }
            else {
                flag = "false";
            }
        }
        if (course1 != "") {
            if (credit1 != "") {
                if (is_integer(credit1) == "false") {
                    flag2 = "false";
                }
                else {
                    int credit1i = stoi(credit1);
                    totalCredit += credit1i;
                    if (gpa1 != "Plan") {
                        totalgpa += possible[gpa1] * credit1i;
                    }
                    else {
                        string nameAndCredit1 = course1 + credit1;
                        confusedCreditList.push_back(nameAndCredit1);
                    }
                }
            }
            else {
                flag = "false";
            }
        }
        if (course2 != "") {
            if (credit2 != "") {
                if (is_integer(credit2) == "false") {
                    flag2 = "false";
                }
                else {
                    int credit2i = stoi(credit2);
                    totalCredit += credit2i;
                    if (gpa2 != "Plan") {
                        totalgpa += possible[gpa2] * credit2i;
                    }
                    else {
                        string nameAndCredit2 = course2 + credit2;
                        confusedCreditList.push_back(nameAndCredit2);
                    }
                }
            }
            else {
                flag = "false";
            }
        }
        if (course3 != "") {
            if (credit3 != "") {
                if (is_integer(credit3) == "false") {
                    flag2 = "false";
                }
                else {
                    int credit3i = stoi(credit3);
                    totalCredit += credit3i;
                    if (gpa3 != "Plan") {
                        totalgpa += possible[gpa3] * credit3i;
                    }
                    else {
                        string nameAndCredit3 = course3 + credit3;
                        confusedCreditList.push_back(nameAndCredit3);
                    }
                }
            }
            else {
                flag = "false";
            }
        }
        if (course4 != "") {
                if (credit4 != "") {
                    if (is_integer(credit4) == "false") {
                        flag2 = "false";
                    }
                    else {
                        int credit4i = stoi(credit4);
                        totalCredit += credit4i;
                        if (gpa4 != "Plan") {
                            totalgpa += possible[gpa4] * credit4i;
                        }
                        else {
                            string nameAndCredit4 = course4 + credit4;
                            confusedCreditList.push_back(nameAndCredit4);
                        }
                    }
                }
                else {
                    flag = "false";
                }
            }
        if (course5 != "") {
                if (credit5 != "") {
                    if (is_integer(credit5) == "false") {
                        flag2 = "false";
                    }
                    else {
                        int credit5i = stoi(credit5);
                        totalCredit += credit5i;
                        if (gpa5 != "Plan") {
                            totalgpa += possible[gpa5] * credit5i;
                        }
                        else {
                            string nameAndCredit5 = course5 + credit5;
                            confusedCreditList.push_back(nameAndCredit5);
                        }
                    }
                }
                else {
                    flag = "false";
                }
            }
        if (course6 != "") {
                if (credit6 != "") {
                    if (is_integer(credit6) == "false") {
                        flag2 = "false";
                    }
                    else {
                        int credit6i = stoi(credit6);
                        totalCredit += credit6i;
                        if (gpa6 != "Plan") {
                            totalgpa += possible[gpa6] * credit6i;
                        }
                        else {
                            string nameAndCredit6 = course6 + credit6;
                            confusedCreditList.push_back(nameAndCredit6);
                        }
                    }
                }
                else {
                    flag = "false";
                }
            }
        if (course7 != "") {
                if (credit7 != "") {
                    if (is_integer(credit7) == "false") {
                        flag2 = "false";
                    }
                    else {
                        int credit7i = stoi(credit7);
                        totalCredit += credit7i;
                        if (gpa7 != "Plan") {
                            totalgpa += possible[gpa7] * credit7i;
                        }
                        else {
                            string nameAndCredit7 = course7 + credit7;
                            confusedCreditList.push_back(nameAndCredit7);
                        }
                    }
                }
                else {
                    flag = "false";
                }
            }
        if (course8 != "") {
                if (credit8 != "") {
                    if (is_integer(credit8) == "false") {
                        flag2 = "false";
                    }
                    else {
                        int credit8i = stoi(credit8);
                        totalCredit += credit8i;
                        if (gpa8 != "Plan") {
                            totalgpa += possible[gpa8] * credit8i;
                        }
                        else {
                            string nameAndCredit8 = course8 + credit8;
                            confusedCreditList.push_back(nameAndCredit8);
                        }
                    }
                }
                else {
                    flag = "false";
                }
            }


        int length = confusedCreditList.size();

        if (targetgpa != "") {
            if (is_double(targetgpa) == "false") {
                flag2 = "false";
            }
        }
        else {flag2 ="false";}
        double diff = -9999999999;
        if (flag == "true" && flag2 == "true") {
            diff = totalgpa - stod(targetgpa) * totalCredit;
        }

        if (flag == "false") {
            QMessageBox::warning(this, "alert", "For each inputted course name, please also provide course credit. Vice versa");
        }
        else if (flag2 == "false") {
            QMessageBox::warning(this, "alert", "Please make sure that the inputted credits and target gpa are numbers");
        }
        else if (length == 0) {
            QMessageBox::warning(this, "alert", "Please leave at least 1 course's gpa blank");
        }
        else if (length == 1) {
            int firstCredit = stoi(confusedCreditList[0].substr(confusedCreditList[0].length()-1,1));
            int remainCredit = firstCredit;
            double maxgpa = (totalgpa + 4 * remainCredit) / totalCredit;
            ui->textEdit->insertPlainText("Your max attainable gpa is " + QString::fromStdString(to_string(maxgpa)) + "\n\n");
            QString html;
            string firstCourse = confusedCreditList[0].substr(0,confusedCreditList[0].length()-1);
            if (firstCourse.length() > maxlength) {firstCourse = firstCourse.substr(0,maxlength);}
            string emptyFlag = "true";
            for (int i = 0; i < possibleLength; i++) {
                double x = possibleList[i];
                double net = diff + x * firstCredit;
                if (net >= 0) {
                    double corGPA = (x * firstCredit + totalgpa) / totalCredit;
                    string gpa1 = possibleR[x];
                    html += QString::fromStdString(to_string(corGPA)) + "\t";
                    html += QString::fromStdString(gpa1) + "\t";
                    html += "\n";
                    emptyFlag = "false";
                }
            }
            if (emptyFlag == "true") {
                ui->textEdit->insertPlainText("Compared to your predicted grades, You either set your GPA target too high or too low. Please adjust it.");
            }
            else {
                string addFront = "\t" + firstCourse + "\n";
                html = QString::fromStdString(addFront) + html;
                ui->textEdit->insertPlainText(html);
            }
        }
        else if (length == 2) {
            int firstCredit = stoi(confusedCreditList[0].substr(confusedCreditList[0].length()-1,1));
            int secondCredit = stoi(confusedCreditList[1].substr(confusedCreditList[1].length()-1,1));
            int remainCredit = firstCredit + secondCredit;
            double maxgpa = (totalgpa + 4 * remainCredit) / totalCredit;
            ui->textEdit->insertPlainText("Your max attainable gpa is " + QString::fromStdString(to_string(maxgpa)) + "\n\n");
            QString html;
            string firstCourse = confusedCreditList[0].substr(0,confusedCreditList[0].length()-1);
            if (firstCourse.length() > maxlength) {firstCourse = firstCourse.substr(0,maxlength);}
            string secondCourse = confusedCreditList[1].substr(0,confusedCreditList[1].length()-1);
            if (secondCourse.length() > maxlength) {secondCourse = secondCourse.substr(0,maxlength);}
            string emptyFlag = "true";
            for (int i = 0; i < possibleLength; i++) {
                double x = possibleList[i];
                for (int j = 0; j < possibleLength; j++) {
                    double y = possibleList[j];
                    double net = diff + x * firstCredit + y * secondCredit;
                    if (net >= 0) {
                        double corGPA = (x * firstCredit + y * secondCredit + totalgpa) / totalCredit;
                        string gpa1 = possibleR[x];
                        string gpa2 = possibleR[y];
                        html += QString::fromStdString(to_string(corGPA)) + "\t";
                        html += QString::fromStdString(gpa1) + "\t";
                        html += QString::fromStdString(gpa2) + "\t";
                        html += "\n";
                        emptyFlag = "false";
                    }
                }
            }
            if (emptyFlag == "true") {
                ui->textEdit->insertPlainText("Compared to your predicted grades, You either set your GPA target too high or too low. Please adjust it.");
            }
            else {
                string addFront = "\t" + firstCourse + "\t" + secondCourse + "\n";
                html = QString::fromStdString(addFront) + html;
                ui->textEdit->insertPlainText(html);
            }
        }
        else if (length == 3) {
            int firstCredit = stoi(confusedCreditList[0].substr(confusedCreditList[0].length()-1,1));
            int secondCredit = stoi(confusedCreditList[1].substr(confusedCreditList[1].length()-1,1));
            int thirdCredit = stoi(confusedCreditList[2].substr(confusedCreditList[2].length()-1,1));
            int remainCredit = firstCredit + secondCredit + thirdCredit;
            double maxgpa = (totalgpa + 4 * remainCredit) / totalCredit;
            ui->textEdit->insertPlainText("Your max attainable gpa is " + QString::fromStdString(to_string(maxgpa)) + "\n\n");
            QString html;
            string firstCourse = confusedCreditList[0].substr(0,confusedCreditList[0].length()-1);
            if (firstCourse.length() > maxlength) {firstCourse = firstCourse.substr(0,maxlength);}
            string secondCourse = confusedCreditList[1].substr(0,confusedCreditList[1].length()-1);
            if (secondCourse.length() > maxlength) {secondCourse = secondCourse.substr(0,maxlength);}
            string thirdCourse = confusedCreditList[2].substr(0,confusedCreditList[2].length()-1);
            if (thirdCourse.length() > maxlength) {thirdCourse = thirdCourse.substr(0,maxlength);}
            string emptyFlag = "true";
            for (int i = 0; i < possibleLength; i++) {
                double x = possibleList[i];
                for (int j = 0; j < possibleLength; j++) {
                    for (int k = 0; k < possibleLength; k++) {
                        double y = possibleList[j];
                        double z = possibleList[k];
                        double net = diff + x * firstCredit + y * secondCredit + z * thirdCredit;
                        if (net >= 0) {
                            double corGPA = (x * firstCredit + y * secondCredit + z * thirdCredit + totalgpa) / totalCredit;
                            string gpa1 = possibleR[x];
                            string gpa2 = possibleR[y];
                            string gpa3 = possibleR[z];
                            html += QString::fromStdString(to_string(corGPA)) + "\t";
                            html += QString::fromStdString(gpa1) + "\t";
                            html += QString::fromStdString(gpa2) + "\t";
                            html += QString::fromStdString(gpa3) + "\t";
                            html += "\n";
                            emptyFlag = "false";
                        }
                    }
                }
            }
            if (emptyFlag == "true") {
                ui->textEdit->insertPlainText("Compared to your predicted grades, You either set your GPA target too high or too low. Please adjust it.");
            }
            else {
                string addFront = "\t" + firstCourse + "\t" + secondCourse + "\t" + thirdCourse + "\n";
                html = QString::fromStdString(addFront) + html;
                ui->textEdit->insertPlainText(html);
            }
        }
        else {
            QMessageBox::warning(this, "alert", "Please leave no more than 3 course's gpa blank");
        }
}

void GPAPlanner::on_pushButton_clicked()
{
    ui->textEdit->setText("");
        ui->targetgpa->setText("");
        ui->course1->setText("");
        ui->credit1->setText("");
        ui->gpa1->setCurrentText("A");
        ui->course2->setText("");
        ui->credit2->setText("");
        ui->gpa2->setCurrentText("A");
        ui->course3->setText("");
        ui->credit3->setText("");
        ui->gpa3->setCurrentText("A");
        ui->course4->setText("");
        ui->credit4->setText("");
        ui->gpa4->setCurrentText("A");
        ui->course5->setText("");
        ui->credit5->setText("");
        ui->gpa5->setCurrentText("A");
        ui->course6->setText("");
        ui->credit6->setText("");
        ui->gpa6->setCurrentText("A");
        ui->course7->setText("");
        ui->credit7->setText("");
        ui->gpa7->setCurrentText("A");
        ui->course8->setText("");
        ui->credit8->setText("");
        ui->gpa8->setCurrentText("A");
}


