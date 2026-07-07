#ifndef TAB5SENSORDATABASE_H
#define TAB5SENSORDATABASE_H

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
class Tab5SensorDatabase;
}

class Tab5SensorDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit Tab5SensorDatabase(QWidget *parent = nullptr);
    ~Tab5SensorDatabase();
    void updateLastDateTimeSql(bool);

private:
    Ui::Tab5SensorDatabase *ui;
    QSqlDatabase qSqlDatabase;

    QLineSeries *illuLine;
    QLineSeries* tempLine;
    QLineSeries* humiLine;
    QChart *pQChart;
    QChartView *pQChartView;
    QDateTimeAxis *pQDateTimeAxis;
    QDateTime   firstDateTime;
    QDateTime   lastDateTime;

private slots:
//    void tab5RecvDataSlot(QStringList& strList);

    void on_pPBDbSearch_clicked();
    void on_pPBDbDelete_clicked();
    void tab5_recv_data_slot(QStringList&);
};

#endif // TAB5SENSORDATABASE_H
