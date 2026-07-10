#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "tab1devicecontrol.h"
#include "tab2socketclient.h"
#include "tab3controlpanel.h"
#include "tab4sensorchart.h"
#include "tab5sensordatabase.h"
#include "tab6webcamera.h"
#include "tab7camopencv.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class mainWidget;
}
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = nullptr);
    ~mainWidget() override;

private:
    Ui::mainWidget *ui;
    Tab1DeviceControl* p_Tab1DeviceControl;
    Tab2SocketClient* p_Tab2SocketClient;
    Tab3ControlPanel* p_Tab3ControlPanel;
    Tab4SensorChart* p_Tab4SensorChart;
    Tab5SensorDatabase* p_Tab5SensorDatabase;
    Tab6WebCamera* p_Tab6WebCamera;
    Tab7CamOpencv* p_Tab7CamOpencv;
};
#endif // MAINWIDGET_H
