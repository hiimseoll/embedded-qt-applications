#include "tab2_sensor.h"
#include "ui_tab2_sensor.h"

tab2_sensor::tab2_sensor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tab2_sensor)
{
    ui->setupUi(this);

    p_illu_line = new QLineSeries(this);
    p_illu_line->setName("조도");

    p_temp_line = new QLineSeries(this);
    p_temp_line->setName("온도");

    p_humi_line = new QLineSeries(this);
    p_humi_line->setName("습도");

    QPen pen;
    pen.setWidth(2);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);

    pen.setBrush(Qt::green);
    p_illu_line->setPen(pen);

    pen.setBrush(Qt::red);
    p_temp_line->setPen(pen);

    pen.setBrush(Qt::blue);
    p_humi_line->setPen(pen);

    p_chart = new QChart();
    p_chart->addSeries(p_illu_line);
    p_chart->addSeries(p_temp_line);
    p_chart->addSeries(p_humi_line);
    p_chart->createDefaultAxes();
    p_chart->axes(Qt::Vertical).constFirst()->setRange(0, 100);

    p_chart_view = new QChartView(p_chart);
    p_chart_view->setRenderHint(QPainter::Antialiasing);

    p_date_time_axis = new QDateTimeAxis;
    p_date_time_axis->setFormat("hh:mm");

    update_last_date_time(false);

    ui->p_chart_view_layout->layout()->addWidget(p_chart_view);
    p_chart_view->chart()->setAxisX(p_date_time_axis, p_illu_line);
    p_chart_view->chart()->setAxisX(p_date_time_axis, p_temp_line);
    p_chart_view->chart()->setAxisX(p_date_time_axis, p_humi_line);

}

tab2_sensor::~tab2_sensor()
{
    delete ui;
}

void tab2_sensor::update_last_date_time(bool flag){
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    if(!flag){
        first_date_time.setDate(date);
        first_date_time.setTime(time);
    }
    last_date_time.setDate(date);
    last_date_time.setTime(time.addSecs(60));
    p_date_time_axis->setRange(first_date_time, last_date_time);
}

void tab2_sensor::tab2_sensor_recv_data_slot(QStringList& str_list){
    QDateTime date_time = QDateTime::currentDateTime();
    p_illu_line->append(date_time.toMSecsSinceEpoch(), str_list[3].toInt());
    p_temp_line->append(date_time.toMSecsSinceEpoch(), str_list[4].toDouble());
    p_humi_line->append(date_time.toMSecsSinceEpoch(), str_list[5].toDouble());

    update_last_date_time(true);
}

void tab2_sensor::on_p_clear_button_clicked(){
    p_illu_line->clear();
    p_temp_line->clear();
    p_humi_line->clear();
}

