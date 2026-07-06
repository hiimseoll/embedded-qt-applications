#ifndef TAB1DEVICECONTROL_H
#define TAB1DEVICECONTROL_H

#include <QWidget>
#include <QTimer>
#include <QCheckBox>
#include <QButtonGroup>
#include <QDial>
#include "ledkeydev.h"

namespace Ui {
class Tab1DeviceControl;
}

class Tab1DeviceControl : public QWidget
{
    Q_OBJECT

public:
    explicit Tab1DeviceControl(QWidget *parent = nullptr);
    ~Tab1DeviceControl();
    LedKeyDev& get_LedKeyDev();
    QDial& get_dialLed();

private slots:
    void on_p_PB_TimerStart_clicked(bool checked);
    void update_dial_value_slot();
    void update_check_box_slot(int);
    void update_key_lcd_slot(int);
    void on_p_CB_TimerValue_currentTextChanged(const QString &arg1);

private:
    int row_cnt;
    int col_cnt;
    int key_cnt;
    Ui::Tab1DeviceControl *ui;
    LedKeyDev* p_LedKeyDev;
    QTimer* p_QTimer;
    QCheckBox** p_QCheckBox;
    QButtonGroup* p_QButtonGroup;
    unsigned char lcd_data;

signals:
    void check_box_sig(int);
};

#endif // TAB1DEVICECONTROL_H
