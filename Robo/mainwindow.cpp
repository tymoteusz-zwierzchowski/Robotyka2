#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Serial Port
    serial.setPortName("COM4");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);

    timeData="t" + QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss");
    serial.write(timeData.toStdString().c_str());

    // Buttons
    send_button = new QPushButton("Send", this);
    switch_button = new QPushButton("Switch to tests", this);
    test1_button = new QPushButton("Load test", this);
    stop_button = new QPushButton("Stop", this);

    // Spin Boxes
    box_x = new QSpinBox(this);
    box_y = new QSpinBox(this);
    box_z = new QSpinBox(this);
    box_xs = new QSpinBox(this);
    box_ys = new QSpinBox(this);
    box_zs = new QSpinBox(this);

    position = new QLabel(this);
    speed = new QLabel(this);

    // Message Log
    messages = new QListWidget(this);

    // Size and locations
    send_button->setGeometry(QRect(QPoint(100, 100), QSize(100, 50)));
    switch_button->setGeometry(QRect(QPoint(200, 100), QSize(100, 50)));
    test1_button->setGeometry(QRect(QPoint(100, 200), QSize(100, 50)));
    stop_button->setGeometry(QRect(QPoint(200, 500), QSize(100, 50)));

    box_x->setGeometry(QRect(QPoint(100, 400), QSize(50, 25)));
    box_y->setGeometry(QRect(QPoint(150, 400), QSize(50, 25)));
    box_z->setGeometry(QRect(QPoint(200, 400), QSize(50, 25)));
    box_xs->setGeometry(QRect(QPoint(100, 300), QSize(50, 25)));
    box_ys->setGeometry(QRect(QPoint(150, 300), QSize(50, 25)));
    box_zs->setGeometry(QRect(QPoint(200, 300), QSize(50, 25)));

    position->setGeometry(QRect(QPoint(150, 380), QSize(50, 20)));
    speed->setGeometry(QRect(QPoint(150, 280), QSize(50, 20)));

    messages->setGeometry(QRect(QPoint(400, 100), QSize(350, 400)));

    // Maximum values
    box_x->setMaximum(360);
    box_y->setMaximum(360);
    box_z->setMaximum(360);
    box_xs->setMaximum(120);
    box_ys->setMaximum(120);
    box_zs->setMaximum(120);
    box_xs->setMinimum(60);
    box_ys->setMinimum(60);
    box_zs->setMinimum(60);

    position->setText("Position");
    speed->setText("Speed");

    // Hide test buttons
    test1_button->hide();
    stop_button->hide();

    // Connects
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readSerial);
    connect(switch_button, &QPushButton::released, this, &MainWindow::switchScreen);
    connect(send_button, &QPushButton::released, this, &MainWindow::handleSendButton); 
    connect(test1_button, &QPushButton::released, this, &MainWindow::handleTestButton1);
    connect(stop_button, &QPushButton::released, this, &MainWindow::handleStopButton);
    connect(messages->model(), SIGNAL(rowsInserted(QModelIndex,int,int)), messages, SLOT(scrollToBottom()));


    messages->addItem("(PC) Start");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleStopButton()
{
    stopTest=1;
    messages->addItem("(PC) Stopping the test...");
}

void MainWindow::switchScreen()
{
    if (switchTest==0){
        test1_button->show();
        stop_button->show();
        switch_button->setText("Switch to manual");
        send_button->hide();
        box_x->hide();
        box_y->hide();
        box_z->hide();
        box_xs->hide();
        box_ys->hide();
        box_zs->hide();
        position->hide();
        speed->hide();
        switchTest=1;
    }
    else {
        test1_button->hide();
        stop_button->hide();
        switch_button->setText("Switch to tests");
        send_button->show();
        box_x->show();
        box_y->show();
        box_z->show();
        box_xs->show();
        box_ys->show();
        box_zs->show();
        position->show();
        speed->show();
        switchTest=0;
    }
}

void MainWindow::handleSendButton()
{
    boxXValue=box_x->value();
    boxYValue=box_y->value();
    boxZValue=box_z->value();
    boxXSpeed=box_xs->value();
    boxYSpeed=box_ys->value();
    boxZSpeed=box_zs->value();
    sendData = QString("%1").arg(boxXValue,3,10,QChar('0'));
    sendData.append(QString(";"));
    sendData.append(QString("%1").arg(boxYValue,3,10,QChar('0')));
    sendData.append(QString(";"));
    sendData.append(QString("%1").arg(boxZValue,3,10,QChar('0')));
    sendData.append(QString(";"));
    sendData.append(QString("%1").arg(boxXSpeed,3,10,QChar('0')));
    sendData.append(QString(";"));
    sendData.append(QString("%1").arg(boxYSpeed,3,10,QChar('0')));
    sendData.append(QString(";"));
    sendData.append(QString("%1").arg(boxZSpeed,3,10,QChar('0')));
    serial.write(sendData.toStdString().c_str());
    messages->addItem("(PC) Speed x: " + QString::number(boxXSpeed) + ", y: " + QString::number(boxYSpeed) + ", z: " + QString::number(boxZSpeed) +  ", Rotation x: " + QString::number(boxXValue) + ", y: " + QString::number(boxYValue) + ", z: " + QString::number(boxZValue));
}



void MainWindow::handleTestButton1()
{
    messages->addItem("(PC) Started new test loop");
    QFile file("C://Users//tym3k//Documents//ROBOIN.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
         while (!in.atEnd() && stopTest != 1) {
             QString line = in.readLine();
             serial.write(line.toStdString().c_str());
             delay();
        }
        if (in.atEnd() && stopTest != 1){
            handleTestButton1();
        }
        stopTest = 0;
        messages->addItem("(PC) Stopped the test");
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(20);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::readSerial()
{
    if(serial.bytesAvailable() > 0){
        count++;
        QByteArray serialData = serial.readAll();
        temp10=temp9;
        temp9=temp8;
        temp8=temp7;
        temp7=temp6;
        temp6=temp5;
        temp5=temp4;
        temp4=temp3;
        temp3=temp2;
        temp2=temp1;
        temp1 = QString::fromStdString(serialData.toStdString());
        //qDebug() << temp;
        //qDebug() << temp1 + ";" + QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss:zzz");
        //QFile file("C:/Users/tym3k/Documents/ROBO2.txt");
              //if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
              //{


        if (count>9){
            QFile file("C:/Users/tym3k/Documents/ROBO3.txt");
            file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
                      // We're going to streaming text to the file
                      QTextStream stream(&file);
                      //stream << temp1 << ";" << QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss:zzz") << '\n';
                      stream << temp10;
                      stream << temp9;
                      stream << temp8;
                      stream << temp7;
                      stream << temp6;
                      stream << temp5;
                      stream << temp4;
                      stream << temp3;
                      stream << temp2;
                      stream << temp1;
                     // file.close();
                      count=0;
        }

              //}
    }
}
