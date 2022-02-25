#ifndef LOADINGPAGE_H
#define LOADINGPAGE_H

#include<QMainWindow>
#include<QLabel>
#include<QProgressBar>

namespace Ui {
class loadingPage;
}

class loadingPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit loadingPage(QWidget *parent = nullptr);
    ~loadingPage();

signals:
    void forcedClosed();

private:
    Ui::loadingPage *ui;
    QLabel *loadingText;
    QProgressBar *loadingBar;
    void updateStatus();
    void closeEvent(QCloseEvent *event);
};

#endif // LOADINGPAGE_H
