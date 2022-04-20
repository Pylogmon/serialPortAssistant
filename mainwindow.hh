#pragma once

#include <QChartView>
#include <QIcon>
#include <QLineSeries>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCharts>
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    //绘图相关
    QLineSeries *series;
    QChart *chart;
    QChartView *chartView; // chartView实例
    //串口相关
    QSerialPort *port;               //串口实例
    QList<QSerialPortInfo> portList; //端口列表
    //构造析构
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int init();                   //软件初始化
    int initSettings();           //初始化设置
    int initSerialPort();         //初始化串口
    int initCharts();             //初始化画布
    int initConnect();            //初始化信号槽
    int setPort();                //设置串口属性
    int scanPort();               //扫描端口
    int openPort();               //打开串口
    int closePort();              //关闭串口
    int showLogInfo(QString mag); //打印日志

  private:
    Ui::MainWindow *ui;
};
