#pragma once

//系统标识符
#ifdef _WIN32
#define SYS 1
#elif __linux__
#define SYS 2
#elif __APPLE__
#define SYS 3
#endif

#include <QDebug>
#include <QFileDialog>
#include <QIcon>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    //状态栏相关
    QLabel *opened;
    QLabel *closed;
    QLabel *red;
    QLabel *green;
    //绘图相关
    // QLineSeries *series;
    // QChart *chart;
    // QChartView *chartView; // chartView实例
    //串口相关
    QSerialPort *port = nullptr;     //串口实例
    QList<QSerialPortInfo> portList; //端口列表
    //时间相关
    int hour;
    int minute;
    int second;
    QString time; //时间头格式化字符串
    //构造析构
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int init();           //软件初始化
    int initStatusBar();  //初始化状态栏
    int initSettings();   //初始化设置
    int initSerialPort(); //初始化串口
    // int initCharts();             //初始化画布
    int initConnect(); //初始化信号槽
    int initUi();
    int setPort();                //设置串口属性
    int scanPort();               //扫描端口
    int openPort();               //打开串口
    int closePort();              //关闭串口
    int showLogInfo(QString mag); //打印日志
    int sendMessage();            //发送消息
    int reciveData();             //接收数据槽函数
    int saveData();               //保存数据
    int getTime();                //获取系统时间
    int changeTextEdit();         //切换输入框
  private:
    Ui::MainWindow *ui;
};
