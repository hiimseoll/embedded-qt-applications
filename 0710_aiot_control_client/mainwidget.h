#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <tab1devicecontrol.h>

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
};
#endif // MAINWIDGET_H
