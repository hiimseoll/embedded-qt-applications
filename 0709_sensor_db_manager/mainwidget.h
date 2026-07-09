#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "tab1_socket.h"
#include "tab2_sensor.h"
#include "tab3_sqlite.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;

private:
    Ui::MainWidget *ui;
    tab1_socket* p_tab1_socket;
    tab2_sensor* p_tab2_sensor;
    tab3_sqlite* p_tab3_sqlite;
};
#endif // MAINWIDGET_H
