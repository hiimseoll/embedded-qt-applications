#ifndef TAB4SENSORCHART_H
#define TAB4SENSORCHART_H

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QDate>
#include <QTime>
#include <QDebug>

namespace Ui {
class Tab4SensorChart;
}

class Tab4SensorChart : public QWidget
{
    Q_OBJECT

public:
    explicit Tab4SensorChart(QWidget *parent = nullptr);
    ~Tab4SensorChart();
    void update_last_date_time(bool);

private slots:
    void tab4_recv_data_slot(QStringList&);

    void on_p_PB_chartClear_clicked();

private:
    Ui::Tab4SensorChart *ui;
    QChart* p_QChart;
    QLineSeries* p_IlluLine;
    QLineSeries* p_TempLine;
    QLineSeries* p_HumiLine;
    QChartView* p_QChartView;
    QDateTimeAxis* p_QDateTimeAxis;
    QDateTime first_date_time;
    QDateTime last_date_time;
};

#endif // TAB4SENSORCHART_H
