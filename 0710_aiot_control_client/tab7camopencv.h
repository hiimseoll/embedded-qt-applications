#ifndef TAB7CAMOPENCV_H
#define TAB7CAMOPENCV_H

#include <QWidget>
#include <webcamthread.h>
namespace Ui {
class Tab7CamOpencv;
}

class Tab7CamOpencv : public QWidget
{
    Q_OBJECT

public:
    explicit Tab7CamOpencv(QWidget *parent = nullptr);
    ~Tab7CamOpencv();
    WebCamThread* getpWebCamThread();

private slots:
    void on_pPBsnapShot_clicked();
    void on_pPBcamStart_clicked(bool checked);
//    void on_pCBrgb_clicked(bool checked);
    void on_pCBrgb_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::Tab7CamOpencv *ui;
    WebCamThread *pWebCamThread;
};

#endif // TAB7CAMOPENCV_H
