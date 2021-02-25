#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QListWidget>
#include <QDateTime>
#include <QLabel>
#include <QFile>
#include <QEventLoop>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void switchScreen();
    void handleSendButton();
    void handleTestButton1();
    void handleStopButton();
    void readSerial();
    void delay();

private:
    Ui::MainWindow *ui;
    QListWidget *messages;
    QPushButton *send_button;
    QPushButton *switch_button;
    QPushButton *test1_button;
    QPushButton *stop_button;
    QSpinBox *box_x;
    QSpinBox *box_y;
    QSpinBox *box_z;
    QSpinBox *box_xs;
    QSpinBox *box_ys;
    QSpinBox *box_zs;
    QLabel *position;
    QLabel *speed;
    int switchTest=0;
    int stopTest=0;
    int boxXValue=0;
    int boxYValue=0;
    int boxZValue=0;
    int boxXSpeed=0;
    int boxYSpeed=0;
    int boxZSpeed=0;
    int count=0;
    QSerialPort serial;
    QString sendData;
    QString timeData;
    QString temp1;
    QString temp2;
    QString temp3;
    QString temp4;
    QString temp5;
    QString temp6;
    QString temp7;
    QString temp8;
    QString temp9;
    QString temp10;
};
#endif // MAINWINDOW_H
