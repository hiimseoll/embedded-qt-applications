#include "webcamthread.h"

int WebCamThread::CAM_MODE = 0;
int WebCamThread::CAM_FUNC_MODE_OFF = 0;
int WebCamThread::RGBCLASSIFY_MODE  = 1;
int WebCamThread::SECURITY_MODE = 2;

WebCamThread::WebCamThread(QObject *parent)
    : QThread(parent)
{
    cnt = 0;
    saveFlag = 0;
    strColor="NONE";
    strColorPre = "";
    camViewFlag = false;
    timerFlag = false;
    pQTimer = new QTimer(this);
    connect(pQTimer, SIGNAL(timeout()), this, SLOT(rgbClassifySlot()));
}

void WebCamThread::run()
{
    int pixelCnt = 0;
    VideoCapture  capture(4);
    if (!capture.isOpened())
    {
        cout << "카메라가 연결되지 않았습니다." << endl;
        exit(1);
    }
    capture.set(CAP_PROP_FRAME_WIDTH,320);
    capture.set(CAP_PROP_FRAME_HEIGHT,240);
    while(camViewFlag) {

        capture.read(frame);

//        put_string(frame, "Count: ", Point(10, 40), cnt);
        fname = "cam_" + to_string(++cnt);
        fname += ".jpg";
        cvtColor(frame, frameQt, COLOR_BGR2RGB);
        int x = frameQt.cols/2;
        int y = frameQt.rows/2;
//        qDebug() << frame.cols ;
 //       qDebug() << frame.rows ;
        if(timerFlag)
        {
            if(CAM_MODE == RGBCLASSIFY_MODE)
            {
                Scalar meanHsv;
                Mat frameRoi, hsvImage;
                frameRoi = frame(Rect((x-32), (y-32), 64, 64));
                cvtColor(frameRoi, hsvImage, COLOR_BGR2HSV);
                meanHsv = mean(hsvImage);
    //            qDebug() << " meanHSV H :" << meanHsv[0] << " meanHSV S :" << meanHsv[1] << " meanHSV V :" << meanHsv[2] ;  //색상

                if( 170 <= meanHsv[0] || meanHsv[0] < 10)   //Red
                    strColor = "RED";
                else if( 50 <= meanHsv[0] && meanHsv[0] < 70)   //Green
                    strColor = "GREEN";
                else if( 110 <= meanHsv[0] && meanHsv[0] < 130) //Blue
                    strColor = "BLUE";
                else
                    strColor = "NONE";

                if(strColor != strColorPre)
                {
                    emit socketSendDataSig("SDH_LIN", "COLOR@" + strColor);
                    strColorPre = strColor;
                }
            }
            else if(CAM_MODE == SECURITY_MODE)
            {
                if(!saveFlag)
                {
                    saveFlag = 1;
                    securityFrame = frame.clone();
                    emit socketSendDataSig("HM_CON", "GASOFF");
                }
                else
                {   saveFlag = 0;
                    securityFrame = abs(securityFrame - frame);
                    uchar* imagePtr = (uchar*)securityFrame.data;
                    for(int y=0;y<securityFrame.rows;y++)
                    {
                        for(int x=0;x<securityFrame.cols;x++)
                        {
                            uchar b = imagePtr[y*securityFrame.step + x * securityFrame.elemSize() + 0];
                            uchar g = imagePtr[y*securityFrame.step + x * securityFrame.elemSize() + 1];
                            uchar r = imagePtr[y*securityFrame.step + x * securityFrame.elemSize() + 2];

                            if( b >30 && g > 30 && r >30)
                            {
                                pixelCnt++;
                                if(pixelCnt >= (frame.cols*frame.rows)*0.1)
                                {
                                    qDebug() << "pixelCnt : " << pixelCnt;

                                    emit socketSendDataSig("HM_CON", "INTRUDER");
                                    QThread::msleep(200);
                                    emit socketSendDataSig("HM_CON", "GASON");
                                    break;
                                }
                            }
                        }
                        if(pixelCnt >= (frame.cols*frame.rows)*0.1)
                        {
                            pixelCnt = 0;
                            break;
                        }
                    }
                }

            }
            timerFlag = false;
        }
        put_string(frameQt, strColor.toStdString(), Point(10, 40));
        line(frameQt,Point((x-32),y), Point((x+32), y), Scalar(255,0,0),1);
        line(frameQt,Point((x),y-32), Point((x), y+32), Scalar(255,0,0),1);
        rectangle(frameQt, Point((x-32),(y-32)), Point((x+32),(y+32)), Scalar(0,255,0),2);
//      QImage qImage(frame.data, frame.cols, frame.rows, QImage::Format_BGR888);
        qImage = QImage(frameQt.data, frameQt.cols, frameQt.rows, QImage::Format_RGB888);
        pCamView->setPixmap(QPixmap::fromImage(qImage));
    }
    capture.release();
    pCamView->setPixmap(QPixmap(":/images/Images/initDisplay_1.png"));
}

// 문자열 출력 함수 - 그림자 효과
void WebCamThread::put_string(Mat &frame, string text, Point pt, int value)
{
    Scalar colorScalar;
    if(value != -1)
        text += to_string(value);
    if(text == "RED")
        colorScalar = {255,0,0};
    else if(text == "GREEN")
        colorScalar = {0,255,0};
    else if(text == "BLUE")
        colorScalar = {0,0,255};
    else
        colorScalar = {128,128,128};
    Point shade = pt + Point(2, 2);
    int font = FONT_HERSHEY_SIMPLEX;
    putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);     // 그림자 효과
    putText(frame, text, pt, font, 0.7, colorScalar, 2);// 작성 문자
}
void WebCamThread::snapShot()
{
//    imwrite(fname,frame);
    qImage.save(QString::fromStdString(fname),"JPG",80);
}

void WebCamThread::rgbTimerStart(int mode)
{
    CAM_MODE = mode;

    pQTimer->start(1000);
//    qDebug() << "start" ;
}

void WebCamThread::rgbTimerStop(int mode)
{

    if(pQTimer->isActive())
    {
        pQTimer->stop();
        CAM_MODE = mode;
    }
//    qDebug() << "stop" ;
}

void WebCamThread::rgbClassifySlot()
{
    timerFlag = true;

}
