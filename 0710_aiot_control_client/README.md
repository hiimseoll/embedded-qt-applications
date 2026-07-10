# 0710_aiot_control_client (7탭 통합 임베디드 제어·모니터링 HMI)

## 1. 구현 개요
리눅스 커널 문자 디바이스 드라이버(`/dev/ledkey`), 비동기 TCP 소켓 통신, 원격 액추에이터 제어판, 실시간 센서 차트, SQLite 데이터베이스, 외부 스트리밍 프로세스 제어(`QProcess`), 그리고 멀티스레드 OpenCV 비전 제어(`QThread`)를 7개 탭으로 통합한 임베디드 제어·모니터링 클라이언트(`AIoTClient`)를 단계별로 구현함.

## 2. 주요 구현 디테일 및 핵심 코드

### ① 커널 디바이스 드라이버 비동기 연동 (`Tab1DeviceControl`, `LedKeyDev`)
```cpp
pQFile = new QFile("/dev/ledkey");
if(!pQFile->open(QIODevice::ReadWrite | QIODevice::Unbuffered))
    return;
ledkeyFd = pQFile->handle();
pQSocketNotifier = new QSocketNotifier(ledkeyFd, QSocketNotifier::Read, this);
connect(pQSocketNotifier, SIGNAL(activated(int)), this, SLOT(readKeyDataSlot(int)));
```
- 폴링 스레드 대신 `QFile::handle()`로 추출한 디바이스 파일 디스크립터를 `QSocketNotifier(Read)`에 등록하여, 하드웨어 스위치 인터럽트 발생 시에만 Qt 이벤트 루프가 `readKeyDataSlot`을 호출하도록 구현함.
- 호스트 PC(Qt 6)와 라즈베리파이 타겟(Qt 5.15) 간의 시그널 차이(`QButtonGroup::idClicked` vs `buttonClicked`)를 `#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)` 매크로로 분기하여 크로스 빌드 호환성을 확보함.

### ② 탭 간 중재자(Mediator) 라우팅 및 원격 제어 (`Tab2SocketClient`, `Tab3ControlPanel`, `Tab4`, `Tab5`)
- 형제 탭 위젯들이 서로를 직접 참조하지 않도록 상위 `mainWidget`에서 Signal/Slot을 중계함.
- 소켓으로 수신된 `LED@값`은 Tab 1의 `QDial` 및 `/dev/ledkey` 하드웨어 출력으로 즉시 전달되고, `LAMP`/`GAS` 응답은 Tab 3 제어판의 `QPalette` 상태로, `SENSOR` 데이터는 Tab 4 실시간 차트와 Tab 5 SQLite DB로 분산 라우팅됨.

### ③ 멀티스레드 OpenCV 비전 판별 및 자동 제어 (`Tab6WebCamera`, `Tab7CamOpencv`, `WebCamThread`)
```cpp
// WebCamThread::run() 내부 (QThread 워커 스레드)
if(rgbTimerFlag) {
    rgbClassify(frame, roiFrame); // 64x64 ROI BGR->HSV 변환 및 색상 판별
    if(strColor != strColorPre) {
        emit socketSendDataSig("[ALLMSG]COLOR@" + strColor);
        strColorPre = strColor;
    }
}
```
- **Tab 6**: `QProcess`로 `mjpg_streamer` 데몬 프로세스의 기동·종료(`start`/`kill`)를 제어하고 `QWebEngineView::grab()`으로 스냅샷을 저장함.
- **Tab 7**: 카메라 프레임 캡처와 영상 연산이 메인 GUI 스레드를 블로킹하지 않도록 `QThread`를 상속한 `WebCamThread`로 분리함.
  - **색상 분류 모드(`RGBCLASSIFY_MODE`)**: 중앙 64×64 ROI의 평균 HSV 값을 계산해 `RED`, `GREEN`, `BLUE`, `DARK`를 판별하고 상태 변경 시에만 소켓 패킷을 자동 송신함.
  - **침입 감지 모드(`SECURITY_MODE`)**: 1초 주기 이전 프레임과의 채널별 절대 차분(`abs`) 픽셀 변화율이 10% 이상일 때 침입(`INTRUDER`)으로 판정하여 경고 및 액추에이터 제어 명령(`[HM_CON]GASON`)을 자동 송신함.

## 3. 기술적 한계 및 해결
1. **타겟 보드에서 `QSocketNotifier`가 `keyNo: 0`을 무한 호출하는 Busy-Loop 해결**:
   - 라즈베리파이 배포 실행 시 스위치를 누르지 않았음에도 슬롯이 초당 수천 번 호출되는 현상이 발생함.
   - 원인 분석 결과 타겟의 `/dev/ledkey`가 `mknod` 문자 디바이스(`c`)가 아닌 일반 빈 파일(`-rw-r--r--`)로 잘못 생성되어, 리눅스 VFS에서 `read()`가 블로킹(대기 큐 휴면)되지 않고 즉시 EOF(`0`)를 반환해 `QSocketNotifier(Read)`가 이를 무한 감지한 것이었음. `/proc/devices`에서 주번호(`230`)를 확인한 뒤 `sudo mknod /dev/ledkey c 230 0`으로 문자 디바이스 노드를 재생성하여 해결함.
2. **다중 OpenCV 환경 링커 충돌 해결**:
   - 로컬의 Anaconda 라이브러리와 시스템 패키지 혼재로 발생한 `undefined reference` 에러를 해결하기 위해 `.pro` 파일에 `/usr/include/opencv4` 및 5개 핵심 모듈(`core`, `imgproc`, `highgui`, `imgcodecs`, `videoio`) 경로를 명시적으로 지정하여 빌드 일관성을 확보함.
