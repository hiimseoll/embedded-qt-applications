#include "tab3controlpanel.h"
#include "ui_tab3controlpanel.h"

Tab3ControlPanel::Tab3ControlPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab3ControlPanel)
{
    ui->setupUi(this);
    paletteOn.setColor(ui->p_PB_Lamp->backgroundRole(), QColor(255,0,0)); // red
    paletteOff.setColor(ui->p_PB_Lamp->backgroundRole(), QColor(0,0,255)); // blue

    ui->p_PB_Lamp->setPalette(paletteOff);
    ui->p_PB_Plug->setPalette(paletteOff);
}

Tab3ControlPanel::~Tab3ControlPanel()
{
    delete ui;
}

void Tab3ControlPanel::on_p_PB_Lamp_clicked(bool checked)
{
    if(checked){
        emit socket_send_data_sig("SDH_QT", "LAMPON");
        ui->p_PB_Lamp->setChecked(false);
    }
    else{
        emit socket_send_data_sig("SDH_QT", "LAMPOFF");
        ui->p_PB_Lamp->setChecked(true);
    }
}


void Tab3ControlPanel::on_p_PB_Plug_clicked(bool checked)
{
    if(checked){
        emit socket_send_data_sig("SDH_QT", "GASON");
        ui->p_PB_Plug->setChecked(false);
    }
    else{
        emit socket_send_data_sig("SDH_QT", "GASOFF");
        ui->p_PB_Plug->setChecked(true);
    }
}


void Tab3ControlPanel::tab3_recv_data_slot(QStringList& str_list){
    if(str_list[2] == "LAMPON"){
        ui->p_PB_Lamp->setChecked(true);
        ui->p_PB_Lamp->setPalette(paletteOn);
    }
    else if(str_list[2] == "LAMPOFF"){
        ui->p_PB_Lamp->setChecked(false);
        ui->p_PB_Lamp->setPalette(paletteOff);
    }
    else if(str_list[2] == "GASON"){
        ui->p_PB_Plug->setChecked(true);
        ui->p_PB_Plug->setPalette(paletteOn);
    }
    else if(str_list[2] == "GASOFF"){
        ui->p_PB_Plug->setChecked(false);
        ui->p_PB_Plug->setPalette(paletteOff);
    }
}

