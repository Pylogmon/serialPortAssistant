#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    closePort();
    delete ui;
}

//初始化
int MainWindow::init()
{
    //设置标题、图标
    this->setWindowTitle("串口小助手");
    this->setWindowIcon(QIcon(":/img/img/icon.svg"));
    //初始化状态栏
    initStatusBar();
    //初始化设置选项
    initSettings();
    //初始化串口
    initSerialPort();
    //初始化绘图画布
    // initCharts();
    //初始化信号槽
    initConnect();
    //初始化ui
    initUi();
    return 0;
}

//初始化状态栏
int MainWindow::initStatusBar()
{
    red = new QLabel;
    green = new QLabel;
    opened = new QLabel("串口已打开");
    closed = new QLabel("串口已关闭");

    red->setStyleSheet(
        "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px; background:red");
    green->setStyleSheet(
        "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px; background:green");

    //添加状态栏永久信息
    ui->statusbar->addPermanentWidget(closed);
    ui->statusbar->addPermanentWidget(red);

    ui->statusbar->addPermanentWidget(opened);
    ui->statusbar->addPermanentWidget(green);

    //隐藏串口打开信息
    opened->hide();
    green->hide();
    return 0;
}

//初始化设置
int MainWindow::initSettings()
{
    //初始化校验位
    ui->checkBox->addItem("无校验");
    ui->checkBox->addItem("奇校验");
    ui->checkBox->addItem("偶校验");
    ui->checkBox->addItem("1校验");
    ui->checkBox->addItem("0校验");
    ui->checkBox->setCurrentIndex(0);
    //初始化波特率
    ui->baudBox->addItem("1200");
    ui->baudBox->addItem("4800");
    ui->baudBox->addItem("9600");
    ui->baudBox->addItem("19200");
    ui->baudBox->addItem("38400");
    ui->baudBox->addItem("57600");
    ui->baudBox->addItem("115200");
    ui->baudBox->setCurrentIndex(2);
    //初始化停止位
    ui->stopBox->addItem("1位");
    ui->stopBox->addItem("1.5位");
    ui->stopBox->addItem("2位");
    ui->stopBox->setCurrentIndex(0);
    //初始化数据位
    ui->dataBox->addItem("5位");
    ui->dataBox->addItem("6位");
    ui->dataBox->addItem("7位");
    ui->dataBox->addItem("8位");
    ui->dataBox->setCurrentIndex(3);
    //初始化文本模式
    ui->textBtn_R->setChecked(true);
    ui->textBtn_S->setChecked(true);
    //禁用关闭串口
    ui->closeBtn->setEnabled(false);
    return 0;
}

//初始化串口
int MainWindow::initSerialPort()
{
    scanPort();
    return 0;
}

//初始化ui
int MainWindow::initUi()
{
    // lineEdit添加输入字符限制
    QRegExp regx("([A-Fa-f0-9]{2}[ ]){" + QString::number(ui->dataBox->currentIndex() + 4) + "}[A-Fa-f0-9]{2}");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit);
    ui->lineEdit->setValidator(validator);
    changeTextEdit();
    return 0;
}
/*
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
*/

//初始化信号槽
int MainWindow::initConnect()
{
    //清空接收区
    connect(ui->clearBtn, &QPushButton::clicked, ui->reciveTextEdit, &QPlainTextEdit::clear);
    //刷新端口列表
    connect(ui->refreshBtn, &QPushButton::clicked, this, &MainWindow::scanPort);
    //发送信息
    connect(ui->sendBtn, &QPushButton::clicked, this, &MainWindow::sendMessage);
    //打开串口
    connect(ui->openBtn, &QPushButton::clicked, this, &MainWindow::openPort);
    //关闭串口
    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::closePort);
    //保存数据
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveData);
    //关于Qt
    connect(ui->qtBtn, &QPushButton::clicked, qApp, &QApplication::aboutQt);
    //切换编辑器
    connect(ui->textBtn_S, &QRadioButton::clicked, this, &MainWindow::changeTextEdit);
    connect(ui->hexBtn_S, &QRadioButton::clicked, this, &MainWindow::changeTextEdit);
    //设置编辑器限制
    connect(ui->dataBox, &QComboBox::currentTextChanged, this, &MainWindow::initUi);
    return 0;
}

//设置串口
int MainWindow::setPort()
{
    port->setBaudRate(ui->baudBox->currentText().toInt());  //设置波特率
    port->setPort(portList.at(ui->comBox->currentIndex())); //设置端口号
    //设置校验位
    switch (ui->checkBox->currentIndex())
    {
    case 0:
        port->setParity(QSerialPort::NoParity); //无校验位
    case 1:
        port->setParity(QSerialPort::OddParity); //奇校验
    case 2:
        port->setParity(QSerialPort::EvenParity); //偶校验
    case 3:
        port->setParity(QSerialPort::MarkParity); // 1校验
    case 4:
        port->setParity(QSerialPort::SpaceParity); // 0校验
    }
    //设置停止位
    switch (ui->stopBox->currentIndex())
    {
    case 0:
        port->setStopBits(QSerialPort::OneStop); // 1位停止位
    case 1:
        port->setStopBits(QSerialPort::OneAndHalfStop); // 1.5位停止位
    case 2:
        port->setStopBits(QSerialPort::TwoStop); //一位停止位
    }
    //设置数据位
    switch (ui->dataBox->currentIndex())
    {
    case 0:
        port->setDataBits(QSerialPort::Data5); //数据位为5位
    case 1:
        port->setDataBits(QSerialPort::Data6); //数据位为6位
    case 2:
        port->setDataBits(QSerialPort::Data7); //数据位为7位
    case 3:
        port->setDataBits(QSerialPort::Data8); //数据位为8位
    }

    port->setFlowControl(QSerialPort::NoFlowControl); //无流控制

    return 0;
}

//扫描可用端口
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

//打开串口
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
        //串口打开后禁用设置项
        ui->comBox->setEnabled(false);
        ui->refreshBtn->setEnabled(false);
        ui->baudBox->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->dataBox->setEnabled(false);
        ui->stopBox->setEnabled(false);
        ui->openBtn->setEnabled(false);
        ui->closeBtn->setEnabled(true);
        //切换状态栏永久显示模块
        opened->show();
        green->show();
        closed->hide();
        red->hide();
    }
    return 0;
}

//关闭串口
int MainWindow::closePort()
{
    //判断串口实例是否存在
    if (port == nullptr)
    {
        return 1;
    }
    //关闭串口
    port->clear();
    port->close();
    delete port;
    port = nullptr;
    //串口设置可用
    ui->comBox->setEnabled(true);
    ui->refreshBtn->setEnabled(true);
    ui->baudBox->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->dataBox->setEnabled(true);
    ui->stopBox->setEnabled(true);
    ui->openBtn->setEnabled(true);
    ui->closeBtn->setEnabled(false);
    //切换状态栏永久显示模块
    closed->show();
    red->show();
    opened->hide();
    green->hide();

    showLogInfo("串口已关闭！");
    return 0;
}

//发送串口数据
int MainWindow::sendMessage()
{
    //检查串口状态
    if (port == nullptr)
    {
        showLogInfo("请先打开串口！");
    }
    else
    {
        //检查text模式还是hex模式
        if (ui->textBtn_S->isChecked())
        {
            if (port->write(ui->sendTextEdit->toPlainText().toUtf8()))
            {
                showLogInfo("发送成功！");
            }
        }
        else
        {
            //构造16进制数据
            QByteArray dataPart;
            QStringList hexData = ui->lineEdit->text().split(" ");
            //检查数据位数
            if (hexData.length() != (ui->dataBox->currentIndex() + 5))
            {
                ui->statusbar->showMessage("数据位数不足！");
                return -1;
            }
            foreach (QString s, hexData)
            {
                dataPart.append(QByteArray::fromHex(s.toLatin1()));
            }
            if (port->write(dataPart))
            {
                showLogInfo("发送成功！");
            }
        }
    }
    return 0;
}

//接收串口数据
int MainWindow::reciveData()
{
    QByteArray info = port->readAll(); //接收串口信息
    //状态栏信息
    ui->statusbar->showMessage("接收到" + QString::number(info.length()) + "Byte数据");

    //打印串口信息
    //检查text模式还是hex模式
    // text模式
    if (ui->textBtn_R->isChecked())
    {
        //检查是否加时间头
        //加时间头
        if (ui->timeCheckBox->isChecked())
        {
            //获取系统时间
            getTime();
            //检查是否换行
            //换行
            if (ui->newLineCheckBox->isChecked())
            {
                ui->reciveTextEdit->appendPlainText(time + info);
            }
            //不换行
            else
            {
                ui->reciveTextEdit->insertPlainText(time + info + ' ');
            }
        }
        //不加时间头
        else
        {
            //换行
            if (ui->newLineCheckBox->isChecked())
            {
                ui->reciveTextEdit->appendPlainText(info);
            }
            //不换行
            else
            {
                ui->reciveTextEdit->insertPlainText(info + ' ');
            }
        }
    }
    // hex模式
    if (ui->hexBtn_R->isChecked())
    {
        QByteArray hexData = info.toHex(); //信息转换为16进制
        //加时间头
        if (ui->timeCheckBox->isChecked())
        {
            getTime();
            //换行
            if (ui->newLineCheckBox->isChecked())
            {
                ui->reciveTextEdit->appendPlainText(time);
                for (int i = 0; i < info.length(); i++)
                {
                    ui->reciveTextEdit->insertPlainText(QString(hexData.at(i * 2)) + QString(hexData.at(i * 2 + 1)) +
                                                        ' ');
                }
            }
            //不换行
            else
            {
                ui->reciveTextEdit->insertPlainText(time);
                for (int i = 0; i < info.length(); i++)
                {
                    ui->reciveTextEdit->insertPlainText(QString(hexData.at(i * 2)) + QString(hexData.at(i * 2 + 1)) +
                                                        ' ');
                }
            }
        }
        //不加时间头
        else
        {
            //换行
            if (ui->newLineCheckBox->isChecked())
            {
                ui->reciveTextEdit->appendPlainText("\n");
                for (int i = 0; i < info.length(); i++)
                {
                    ui->reciveTextEdit->insertPlainText(QString(hexData.at(i * 2)) + QString(hexData.at(i * 2 + 1)) +
                                                        ' ');
                }
            }
            //不换行
            else
            {
                for (int i = 0; i < info.length(); i++)
                {
                    ui->reciveTextEdit->insertPlainText(QString(hexData.at(i * 2)) + QString(hexData.at(i * 2 + 1)) +
                                                        ' ');
                }
            }
        }
    }
    return 0;
}

//保存数据
int MainWindow::saveData()
{
    //弹出窗口获取保存路径
    QString filename = QFileDialog::getSaveFileName(this, tr("选择保存路径"), "", tr("*.txt"));
    //打开文件
    QFile file(filename);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    //获取日志信息
    QString saveData = ui->reciveTextEdit->toPlainText();
    //写入日志信息
    if (file.write(saveData.toUtf8()) > 0)
    {
        //弹出成功对话框
        QMessageBox::information(this, tr("保存数据"), tr("保存成功！"));
    }

    return 0;
}

//状态栏打印信息
int MainWindow::showLogInfo(QString msg)
{
    ui->statusbar->showMessage(msg);
    return 0;
}

//获取当前系统时间
int MainWindow::getTime()
{
    QTime now = QTime::currentTime();
    //格式化字符串，两位数字，不足补0
    QString hour = QString("%1").arg(now.hour(), 2, 10, QChar('0'));
    QString minute = QString("%1").arg(now.minute(), 2, 10, QChar('0'));
    QString second = QString("%1").arg(now.second(), 2, 10, QChar('0'));
    // int hour = now.hour();
    // int minute = now.minute();
    // int second = now.second();
    this->time = "[" + hour + ":" + minute + ":" + second + "] ";
    return 0;
}

//切换编辑器
int MainWindow::changeTextEdit()
{
    // text模式
    if (ui->textBtn_S->isChecked())
    {
        ui->lineEdit->hide();
        ui->sendTextEdit->show();
    }
    // hex模式
    else
    {
        ui->lineEdit->show();
        ui->sendTextEdit->hide();
    }
    return 0;
}
