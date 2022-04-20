#pragma once

#include <QChartView>
#include <QLineSeries>
#include <QMainWindow>
#include <QtCharts>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

  private:
    Ui::MainWindow *ui;
};
