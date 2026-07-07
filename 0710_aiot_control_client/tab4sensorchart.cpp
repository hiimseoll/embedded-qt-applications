#include "tab4sensorchart.h"
#include "ui_tab4sensorchart.h"

Tab4SensorChart::Tab4SensorChart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab4SensorChart)
{
    ui->setupUi(this);

    p_IlluLine = new QLineSeries(this);
    p_IlluLine->setName("조도");

    p_TempLine = new QLineSeries(this);
    p_TempLine->setName("온도");

    p_HumiLine = new QLineSeries(this);
    p_HumiLine->setName("습도");

    QPen pen;
    pen.setWidth(2);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);

    pen.setBrush(Qt::green);
    p_IlluLine->setPen(pen);

    pen.setBrush(Qt::red);
    p_TempLine->setPen(pen);

    pen.setBrush(Qt::blue);
    p_HumiLine->setPen(pen);

    p_QChart = new QChart();
    p_QChart->addSeries(p_IlluLine);
    p_QChart->addSeries(p_TempLine);
    p_QChart->addSeries(p_HumiLine);
    p_QChart->createDefaultAxes();
    p_QChart->axes(Qt::Vertical).constFirst()->setRange(0, 100);

    p_QChartView = new QChartView(p_QChart);
    p_QChartView->setRenderHint(QPainter::Antialiasing);

    p_QDateTimeAxis = new QDateTimeAxis;
    p_QDateTimeAxis->setFormat("hh:mm");

    update_last_date_time(false);

    ui->p_QChartViewLayout->layout()->addWidget(p_QChartView);
    p_QChartView->chart()->setAxisX(p_QDateTimeAxis, p_IlluLine);
    p_QChartView->chart()->setAxisX(p_QDateTimeAxis, p_TempLine);
    p_QChartView->chart()->setAxisX(p_QDateTimeAxis, p_HumiLine);
#if 0
    QDateTime date_time = QDateTime::currentDateTime();
    p_IlluLine->append(date_time.toMSecsSinceEpoch(), 30);
    date_time = date_time.addSecs(60);
    p_IlluLine->append(date_time.toMSecsSinceEpoch(), 80);
    date_time = date_time.addSecs(60);
    p_IlluLine->append(date_time.toMSecsSinceEpoch(), 50);
#endif
}

Tab4SensorChart::~Tab4SensorChart()
{
    delete ui;
}

void Tab4SensorChart::update_last_date_time(bool flag){
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    if(!flag){
        first_date_time.setDate(date);
        first_date_time.setTime(time);
    }
    last_date_time.setDate(date);
    last_date_time.setTime(time.addSecs(60));
    p_QDateTimeAxis->setRange(first_date_time, last_date_time);
}

void Tab4SensorChart::tab4_recv_data_slot(QStringList str_list){
    QDateTime date_time = QDateTime::currentDateTime();
    p_IlluLine->append(date_time.toMSecsSinceEpoch(), str_list[3].toInt());
    p_TempLine->append(date_time.toMSecsSinceEpoch(), str_list[4].toDouble());
    p_HumiLine->append(date_time.toMSecsSinceEpoch(), str_list[5].toDouble	());

    update_last_date_time(true);
}
void Tab4SensorChart::on_p_PB_chartClear_clicked()
{
    p_IlluLine->clear();
}

