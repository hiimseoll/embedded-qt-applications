#ifndef TAB3_SQLITE_H
#define TAB3_SQLITE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QDate>
#include <QTime>
#include <QDebug>

namespace Ui {
class tab3_sqlite;
}

class tab3_sqlite : public QWidget
{
    Q_OBJECT

public:
    explicit tab3_sqlite(QWidget *parent = nullptr);
    ~tab3_sqlite();
    void update_last_date_time_sql(bool);

private slots:
    void tab3_sqlite_recv_data_slot(QStringList&);
    void on_p_search_button_clicked();
    void on_p_delete_button_clicked();

private:
    Ui::tab3_sqlite *ui;
    QSqlDatabase sql_database;
    QChart* p_chart;
    QLineSeries* p_illu_line;
    QLineSeries* p_temp_line;
    QLineSeries* p_humi_line;
    QChartView* p_chart_view;
    QDateTimeAxis* p_date_time_axis;
    QDateTime first_date_time;
    QDateTime last_date_time;

};

#endif // TAB3_SQLITE_H
