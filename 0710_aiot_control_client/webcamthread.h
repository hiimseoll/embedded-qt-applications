#ifndef WEBCAMTHREAD_H
#define WEBCAMTHREAD_H

#include <QThread>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
class WebCamThread : public QThread
{
    Q_OBJECT
    void run();
    int cnt;
    bool saveFlag;
    string fname;
    QString strColor, strColorPre;
    Mat frame, frameQt, securityFrame;
    QImage qImage;
    QTimer *pQTimer;
    bool timerFlag;
    void put_string(Mat &frame, string text, Point pt, int value = -1);

public:
    WebCamThread(QObject *parent = nullptr);
    bool camViewFlag;
    QLabel *pCamView;
    static int CAM_MODE;
    static int CAM_FUNC_MODE_OFF;
    static int RGBCLASSIFY_MODE;
    static int SECURITY_MODE;
    void snapShot();
    void rgbTimerStart(int);
    void rgbTimerStop(int);

private slots:
    void rgbClassifySlot();

signals:
    void socketSendDataSig(QString, QString);
};

#endif // WEBCAMTHREAD_H
