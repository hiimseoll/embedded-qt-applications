#include "tab3_sqlite.h"
#include "ui_tab3_sqlite.h"

tab3_sqlite::tab3_sqlite(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tab3_sqlite)
{
    ui->setupUi(this);
    sql_database =QSqlDatabase::addDatabase("QSQLITE");
    sql_database.setDatabaseName("aiot.db");

    if(sql_database.open()){
        qDebug() << "database connection succeded.";
    }
    else{
        qDebug() << "database connection failed.";
    }

    QString str_query =  "create table sensor_tb ("
                       "name varchar(10),"
                       "date DATETIME primary key,"
                       "illu varchar(10),"
                       "temp varchar(10),"
                       "humi varchar(10))";

    QSqlQuery sql_query;
    if(sql_query.exec(str_query)){
        qDebug() << "table created.";
    }

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

    update_last_date_time_sql(false);

    ui->p_chart_view_layout->layout()->addWidget(p_chart_view);
    p_chart_view->chart()->setAxisX(p_date_time_axis, p_illu_line);
    p_chart_view->chart()->setAxisX(p_date_time_axis, p_temp_line);
    p_chart_view->chart()->setAxisX(p_date_time_axis, p_humi_line);


}

void tab3_sqlite::update_last_date_time_sql(bool flag){
    QString str_from = ui->p_from_date_time->dateTime().toString("yyyy/MM/dd hh:mm:ss");
    QString str_until = ui->p_until_date_time->dateTime().toString("yyyy/MM/dd hh:mm:ss");

    if(!flag){
        first_date_time = QDateTime::fromString(str_from, "yyyy/MM/dd hh:mm:ss");
        last_date_time = QDateTime::fromString(str_until, "yyyy/MM/dd hh:mm:ss");
    }
    p_date_time_axis->setRange(first_date_time, last_date_time);
}

void tab3_sqlite::tab3_sqlite_recv_data_slot(QStringList& strList){
    QDateTime dateTime = QDateTime::currentDateTime();
    QString name = strList[1];
    QString strIllu = strList[3];  //조도
    QString strTemp = strList[4];
    QString strHumi = strList[5];

    QString strQuery = "insert into sensor_tb(name, date, illu, temp, humi) values('" + name + "' , '" + dateTime.toString("yyyy/MM/dd hh:mm:ss") + "' , '" + strIllu + "', '" + strTemp + "' , '" + strHumi + "') ";
    QSqlQuery qSqlQuery;
    if(qSqlQuery.exec(strQuery))
        qDebug() << "Insert Query Ok";
}
void tab3_sqlite::on_p_search_button_clicked(){
    ui->p_table->clearContents();
    p_illu_line->clear();
    p_temp_line->clear();
    p_humi_line->clear();

    int rowCount = 0;
    QDateTime fromDateTime = ui->p_from_date_time->dateTime();
    QString strFromDateTime = fromDateTime.toString("yyyy/MM/dd hh:mm:ss");

    QDateTime toDateTime = ui->p_until_date_time->dateTime();
    QString strToDateTime = toDateTime.toString("yyyy/MM/dd hh:mm:ss");

    update_last_date_time_sql(false);

    QString strQuery = " select * from sensor_tb where '" + strFromDateTime + "' <= date AND date < '" + strToDateTime + "' ";
    QSqlQuery qSqlQuery;
    if(qSqlQuery.exec(strQuery))
    {
        qDebug() << "Select Query Ok";
        while(qSqlQuery.next())
        {
            QTableWidgetItem *pQTableWidgetItemId = new QTableWidgetItem();
            QTableWidgetItem *pQTableWidgetItemDate = new QTableWidgetItem();
            QTableWidgetItem *pQTableWidgetItemIllu = new QTableWidgetItem();
            QTableWidgetItem *pQTableWidgetItemTemp = new QTableWidgetItem();
            QTableWidgetItem *pQTableWidgetItemHumi = new QTableWidgetItem();

            pQTableWidgetItemId->setText(qSqlQuery.value("name").toString());
            pQTableWidgetItemDate->setText(qSqlQuery.value("date").toString());
            pQTableWidgetItemIllu->setText(qSqlQuery.value("illu").toString());
            pQTableWidgetItemTemp->setText(qSqlQuery.value("temp").toString());
            pQTableWidgetItemHumi->setText(qSqlQuery.value("humi").toString());

            ui->p_table->setRowCount(rowCount+1);
            ui->p_table->setItem(rowCount, 0, pQTableWidgetItemId);
            ui->p_table->setItem(rowCount, 1, pQTableWidgetItemDate);
            ui->p_table->setItem(rowCount, 2, pQTableWidgetItemIllu);
            ui->p_table->setItem(rowCount, 3, pQTableWidgetItemTemp);
            ui->p_table->setItem(rowCount, 4, pQTableWidgetItemHumi);

            QDateTime xValue = QDateTime::fromString(pQTableWidgetItemDate->text(), "yyyy/MM/dd hh:mm:ss");
            p_illu_line->append(xValue.toMSecsSinceEpoch(), pQTableWidgetItemIllu->text().toInt());
            p_temp_line->append(xValue.toMSecsSinceEpoch(), pQTableWidgetItemTemp->text().toDouble());
            p_humi_line->append(xValue.toMSecsSinceEpoch(), pQTableWidgetItemHumi->text().toDouble());

            rowCount++;
        }
        ui->p_table->resizeColumnToContents(0);
        ui->p_table->resizeColumnToContents(1);
        ui->p_table->resizeColumnToContents(2);
        ui->p_table->resizeColumnToContents(3);
        ui->p_table->resizeColumnToContents(4);
    }

}


void tab3_sqlite::on_p_delete_button_clicked(){
    QDateTime fromDateTime = ui->p_from_date_time->dateTime();
    QString strFromDateTime = fromDateTime.toString("yyyy/MM/dd hh:mm:ss");

    QDateTime toDateTime = ui->p_until_date_time->dateTime();
    QString strToDateTime = toDateTime.toString("yyyy/MM/dd hh:mm:ss");

    QString strQuery = "delete from sensor_tb where '" + strFromDateTime + "' <= date AND date < '" + strToDateTime + "' ";
    QSqlQuery qSqlQuery;
    if(qSqlQuery.exec(strQuery)){
        qDebug() << "delete query ok";
    }

    ui->p_table->clearContents();
    p_illu_line->clear();
    p_temp_line->clear();
    p_humi_line->clear();
}


tab3_sqlite::~tab3_sqlite(){
    delete ui;
}