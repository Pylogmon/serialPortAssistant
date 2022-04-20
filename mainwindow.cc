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
    return 0;
}

int MainWindow::closePort()
{
    delete port;
    return 0;
}

int MainWindow::showLogInfo(QString msg)
{
    ui->statusbar->showMessage(msg);
    return 0;
}
