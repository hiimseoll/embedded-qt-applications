#include "tab1devicecontrol.h"
#include "ui_tab1devicecontrol.h"

Tab1DeviceControl::Tab1DeviceControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab1DeviceControl)
{
    ui->setupUi(this);
    p_LedKeyDev = new LedKeyDev(this);
    p_QTimer = new QTimer(this);
    p_QButtonGroup = new QButtonGroup(this);

    lcd_data = 0x00;

    row_cnt = ui->gridLayout->rowCount();
    col_cnt = ui->gridLayout->columnCount();
    key_cnt = row_cnt * col_cnt;

    p_QCheckBox = new QCheckBox*[key_cnt];

    for(int i = 0; i < row_cnt; i++){
        for(int n = 0; n < col_cnt; n++){
            p_QCheckBox[key_cnt - 1 - (i * col_cnt + n)] = dynamic_cast<QCheckBox*>(ui->gridLayout->itemAtPosition(i, n)->widget());
        }
    }

    for(int i = 0; i < key_cnt; i++){
        p_QButtonGroup->addButton(p_QCheckBox[i], i + 1);
    }
    p_QButtonGroup->setExclusive(false);

    connect(ui->p_dialLed, SIGNAL(valueChanged(int)), p_LedKeyDev, SLOT(writeLedDataSlot(int)));
    connect(ui->p_PB_Quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(p_QTimer, SIGNAL(timeout()), this, SLOT(update_dial_value_slot()));
    connect(p_LedKeyDev, SIGNAL(updateKeyDataSig(int)), this, SLOT(update_check_box_slot(int)));
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    connect(p_QButtonGroup, SIGNAL(idClicked(int)), this, SLOT(update_key_lcd_slot(int)));
#else
    connect(p_QButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(update_key_lcd_slot(int)));
#endif
}

Tab1DeviceControl::~Tab1DeviceControl()
{
    delete[] p_QCheckBox;
    delete ui;
}

void Tab1DeviceControl::on_p_PB_TimerStart_clicked(bool checked)
{
    if(checked){
        int timer_value = ui->p_CB_TimerValue->currentText().toInt(); // QString .toInt();

        ui->p_PB_TimerStart->setText("TimerStop");
        p_QTimer->start(timer_value);
    }
    else{
        ui->p_PB_TimerStart->setText("TimerStart");
        p_QTimer->stop();
    }

}

void Tab1DeviceControl::update_dial_value_slot(){

    int dial_value = ui->p_dialLed->value();

    dial_value++;

    if(dial_value > ui->p_dialLed->maximum()){
        dial_value = 0;
    }

    ui->p_dialLed->setValue(dial_value);
}

void Tab1DeviceControl::update_check_box_slot(int key_num){
    for(int i = 0; i < key_cnt; i++){
        if(key_num == i + 1){

            p_QCheckBox[i]->setChecked(!(p_QCheckBox[i]->isChecked()));
            // lcd_data ^= 0x01 << (key_num - 1);

            // ui->p_lcdNumber_key->display(lcd_data);
            update_key_lcd_slot(key_num);
        }
    }
    //ui->p_dialLed->setValue(lcd_data);
}

void Tab1DeviceControl::update_key_lcd_slot(int val){

    lcd_data ^= 0x01 << (val - 1);
    ui->p_lcdNumber_key->display(lcd_data);
#if 1
    emit check_box_sig(lcd_data);
#else
    ui->p_dialLed->setValue(lcd_data); // need to check
#endif
}

void Tab1DeviceControl::on_p_CB_TimerValue_currentTextChanged(const QString &arg1)
{
    if(p_QTimer->isActive()){
        p_QTimer->stop();
        p_QTimer->start(arg1.toInt());
    }
}

LedKeyDev& Tab1DeviceControl::get_LedKeyDev(){
    return *p_LedKeyDev;
}

QDial& Tab1DeviceControl::get_dialLed(){
    return *this->ui->p_dialLed;
}