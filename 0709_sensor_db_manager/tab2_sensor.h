#ifndef TAB2_SENSOR_H
#define TAB2_SENSOR_H

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QDate>
#include <QTime>
#include <QDebug>

namespace Ui {
class tab2_sensor;
}

class tab2_sensor : public QWidget
{
    Q_OBJECT

public:
    explicit tab2_sensor(QWidget *parent = nullptr);
    ~tab2_sensor();
    void update_last_date_time(bool);

private slots:
    void tab2_sensor_recv_data_slot(QStringList&);

    void on_p_clear_button_clicked();

private:
    Ui::tab2_sensor *ui;
    QChart* p_chart;
    QLineSeries* p_illu_line;
    QLineSeries* p_temp_line;
    QLineSeries* p_humi_line;
    QChartView* p_chart_view;
    QDateTimeAxis* p_date_time_axis;
    QDateTime first_date_time;
    QDateTime last_date_time;
};

#endif // TAB2_SENSOR_H
