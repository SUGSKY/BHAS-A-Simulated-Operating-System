#include<QDebug>
#include<QLabel>
#include<QPixmap>
#include<QRandomGenerator>
#include<QTimer>
#include<QVBoxLayout>
#include "loadingpage.h"
#include "ui_loadingpage.h"

//global Variable
const int speed = 50;
const double prob = 0.7;
int counter = 0;

loadingPage::loadingPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loadingPage)
{
    ui->setupUi(this);
    //Setup
    this->setStyleSheet("background-color: white;");
    QPixmap LOGO_icon(":/img/LOGO.png");


    //widgets
    QLabel *LOGO = new QLabel(this);
    loadingText = new QLabel(this);
    loadingBar = new QProgressBar(this);
    QTimer *updateStatusTimer = new QTimer(this);

    //widget setup
    LOGO->setFixedSize(675,450);
    LOGO->setPixmap(LOGO_icon.scaled(675, 450, Qt::KeepAspectRatio));
    loadingText->setFixedSize(150,35);
    loadingText->setAlignment(Qt::AlignCenter);
    loadingBar->setRange(0, 1000);
    loadingBar->setValue(0);
    loadingBar->setFixedSize(400, 40);
    loadingBar->setTextVisible(0);
    loadingBar->setStyleSheet("QProgressBar{border:1px solid black; padding: 1px; background: white} QProgressBar::chunk{background: solid black;}");

    //layout
    QWidget *progressWidget = new QWidget;
    QVBoxLayout *progressLayout = new QVBoxLayout(progressWidget);
    progressWidget->setFixedSize(450, 100);
    progressLayout->addWidget(loadingBar, 0, Qt::AlignTop | Qt::AlignHCenter);
    progressLayout->addSpacing(15);
    progressLayout->addWidget(loadingText, 0, Qt::AlignBottom | Qt::AlignHCenter);

    ui->mainLayout->addSpacing(50);
    ui->mainLayout->addWidget(LOGO, 0, Qt::AlignTop | Qt::AlignHCenter);
    ui->mainLayout->addWidget(progressWidget, 0, Qt::AlignCenter);


    //connection
    connect(updateStatusTimer, &QTimer::timeout, this, &loadingPage::updateStatus);
    updateStatusTimer->start(50);
}

loadingPage::~loadingPage()
{
    delete ui;
}

void loadingPage::updateStatus(){
    if(counter/10 == 0){
        loadingText->setText("Loading");
    }else if(counter/10 == 1){
        loadingText->setText("Loading.");
    }else if(counter/10 == 2){
        loadingText->setText("Loading..");
    }else if(counter/10 == 3){
        loadingText->setText("Loading...");
    }
    counter = (counter+1)%40;
    if(loadingBar->value() >= 1000){
        parentWidget()->show();
        this->hide();
    }
    if(QRandomGenerator::global()->generateDouble() < prob) loadingBar->setValue(loadingBar->value()+speed);
}

void loadingPage::closeEvent(QCloseEvent *event){
    emit forcedClosed();
}
