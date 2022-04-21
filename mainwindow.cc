#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::init()
{
    //设置标题、图标
    this->setWindowTitle("串口小助手");
    this->setWindowIcon(QIcon(":/img/img/icon.svg"));
    //初始化设置选项
    initSettings();
    //初始化串口
    initSerialPort();
    //初始化绘图画布
    initCharts();
    //初始化信号槽
    initConnect();
    // showLogInfo();
    return 0;
}

int MainWindow::initSettings()
{
    ui->checkBox->addItem("无校验");
    ui->checkBox->addItem("奇校验");
    ui->checkBox->addItem("偶校验");
    ui->checkBox->setCurrentIndex(0);
    ui->baudBox->addItem("1200");
    ui->baudBox->addItem("4800");
    ui->baudBox->addItem("9600");
    ui->baudBox->addItem("19200");
    ui->baudBox->addItem("38400");
    ui->baudBox->addItem("57600");
    ui->baudBox->addItem("115200");
    ui->baudBox->setCurrentIndex(2);
    ui->textBtn_R->setChecked(true);
    ui->textBtn_S->setChecked(true);
    ui->closeBtn->setEnabled(false);
    return 0;
}

int MainWindow::initSerialPort()
{
    scanPort();
    return 0;
}

int MainWindow::initCharts()
{
    series = new QLineSeries();
    series->append(300, 0);
    series->append(0, 50);

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->canvas->addWidget(chartView, 20, 80);

    return 0;
}

int MainWindow::initConnect()
{
    //清空接收区
    connect(ui->clearBtn, &QPushButton::clicked, ui->reciveTextEdit, &QPlainTextEdit::clear);
    //刷新端口列表
    connect(ui->refreshBtn, &QPushButton::clicked, this, &MainWindow::scanPort);

    connect(ui->sendBtn, &QPushButton::clicked, this, &MainWindow::sendMessage);

    connect(ui->openBtn, &QPushButton::clicked, this, &MainWindow::openPort);

    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::closePort);

    return 0;
}

int MainWindow::setPort()
{
    port->setBaudRate(ui->baudBox->currentText().toInt());  //设置波特率
    port->setPort(portList.at(ui->comBox->currentIndex())); //设置端口号
    //设置校验位
    switch (ui->checkBox->currentIndex())
    {
    case 0:
        port->setParity(QSerialPort::NoParity);
    case 1:
        port->setParity(QSerialPort::OddParity);
    case 2:
        port->setParity(QSerialPort::EvenParity);
    }

    return 0;
}

int MainWindow::scanPort()
{
    ui->comBox->clear(); //清空下拉菜单

    portList = QSerialPortInfo::availablePorts(); //获取端口列表

    //添加下拉菜单
    for (QSerialPortInfo &i : portList)
    {
        ui->comBox->addItem(i.portName() + ": " + i.description());
    }
    //打印日志信息
    showLogInfo("刷新端口列表成功！ 获取到" + QString::number(portList.size()) + "个有效端口");
    return 0;
}

int MainWindow::openPort()
{
    port = new QSerialPort;
    setPort();
    //检查可用端口
    if (portList.size() == 0)
    {
        showLogInfo("无可用端口！，请刷新端口信息");
    }
    else
    {
        //如果串口已经打开，则先关闭串口
        if (port->isOpen())
        {
            port->clear();
            port->close();
        }
        // Linux下需要获取串口的权限
        int sys = SYS;
        if (sys == 2)
        {
            QString command = "pkexec chmod 777 /dev/" + portList.at(ui->comBox->currentIndex()).portName();
            system(command.toStdString().data());
        }
        //尝试打开串口
        if (!port->open(QIODevice::ReadWrite))
        {
            //打开失败，打印日志信息
            showLogInfo("串口打开失败，请检查端口信息！");
            return -1;
        }

        //连接下位机发送信号
        connect(port, &QSerialPort::readyRead, this, &MainWindow::reciveData);
        //打印日志信息
        showLogInfo("串口已开启，正在监听串口……");
        ui->comBox->setEnabled(false);
        ui->baudBox->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->openBtn->setEnabled(false);
        ui->closeBtn->setEnabled(true);
    }

    return 0;
}

int MainWindow::closePort()
{
    port->clear();
    port->close();
    delete port;

    ui->comBox->setEnabled(true);
    ui->baudBox->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->openBtn->setEnabled(true);
    ui->closeBtn->setEnabled(false);

    showLogInfo("串口已关闭！");
    return 0;
}

int MainWindow::sendMessage()
{
    if (port == nullptr || !port->isOpen())
    {
        showLogInfo("请先打开串口！");
    }
    else
    {
        if (port->write(ui->sendTextEdit->toPlainText().toUtf8()))
        {
            showLogInfo("发送成功！");
        }
    }
    return 0;
}

int MainWindow::reciveData()
{
    QByteArray info = port->readAll(); //接收串口信息
    QByteArray hexData = info.toHex(); //信息转换为16进制
    // QByteArray textData = info.to
    //打印串口信息
    if (ui->textBtn_R->isChecked())
    {
        ui->reciveTextEdit->appendPlainText(info);
    }
    if (ui->hexBtn_R->isChecked())
    {
        ui->reciveTextEdit->appendPlainText(hexData);
    }
    return 0;
}

int MainWindow::showLogInfo(QString msg)
{
    ui->statusbar->showMessage(msg);
    return 0;
}
