# Embedded Qt Control & Monitoring Applications

본 레포지토리는 임베디드 리눅스(Raspberry Pi 4B) 및 마이크로컨트롤러(STM32F411) 연동 환경에서 하드웨어 디바이스 제어, 비동기 시리얼·소켓 통신, 실시간 시계열 시각화, SQLite 데이터베이스 로깅, 멀티스레드 비전 처리를 C++/Qt(Qt5·Qt6 호환)로 구현한 프로젝트 모음입니다.

단순한 데스크톱 UI 구성을 넘어, 리눅스 커널 디바이스 드라이버(`/dev/ledkey`)의 파일 디스크립터를 이벤트 루프(`QSocketNotifier`)에 통합하고, 워커 스레드(`QThread`) 분리로 GUI 응답성을 보장하는 임베디드 제어기 어플리케이션(HMI) 아키텍처를 다룹니다.

---

## 1. 주요 구현 항목

- **커널 디바이스 드라이버 비동기 연동**: 폴링(Polling) 루프에 의한 CPU 점유와 UI 프리징을 방지하기 위해 문자 디바이스 파일 디스크립터(`QFile::handle()`)를 `QSocketNotifier(Read)`에 등록하여 인터럽트 기반으로 하드웨어 입력을 처리.
- **비동기 시리얼(`QSerialPort`) 및 소켓(`QTcpSocket`) 프로토콜 파싱**: 바이트 스트림 조각화(Fragmentation)에 대응하는 수신 버퍼링 및 구분자(`$...#`, `@`) 기반 프레임 파서 구현, 상위 컨테이너(`mainWidget`)의 Signal/Slot 중계를 통한 탭 간 결합도 분리.
- **실시간 데이터 시각화 및 로컬 DB 영속화**: `QChartView`·`QLineSeries`·`QDateTimeAxis` 기반 다중 센서 실시간 시계열 렌더링 및 `QSqlDatabase(QSQLITE)` 연동을 통한 기간별 이력 조회·차트 복원.
- **멀티스레드 비전 처리 및 서브프로세스 제어**: `QThread`를 상속한 워커 스레드에서 OpenCV 영상 처리(ROI HSV 색상 분류, 차영상 침입 감지)를 수행하고, `QProcess`로 외부 스트리밍 데몬(`mjpg_streamer`) 생명주기를 제어.

---

## 2. 모듈별 세부 구현 사항

| 디렉터리 | 주제 | 핵심 구현 내용 |
| :--- | :--- | :--- |
| [`0421_stm32_serial_monitor`](./0421_stm32_serial_monitor) | **STM32 UART CLI 제어 및 센서 모니터** | `QSerialPort` 기반 비동기 수신 버퍼링(`m_buffer`), `$count,id:type:val#` 프레이밍 파서, `QProcess("lsof")` 포트 점유 검사, 동적 프로퍼티 기반 QSS 상태 전환 |
| [`0702_qt_vi_editor`](./0702_qt_vi_editor) | **C/C++ 구문 강조 코드 뷰어** | `QFileSystemModel` 디렉터리 트리 탐색기, `QPlainTextEdit` 서브클래싱 기반 줄 번호 여백(`LineNumberArea`) 커스텀 페인팅, `QSyntaxHighlighter` 정규표현식 구문 강조 |
| [`0703_webcam_stream_viewer`](./0703_webcam_stream_viewer) | **4채널 MJPEG 스트리밍 모니터링 뷰어** | 2×2 그리드 다중 `QWebEngineView` 인스턴스 구성을 통한 4채널 HTTP MJPEG 카메라 스트림 동시 모니터링 |
| [`0709_sensor_db_manager`](./0709_sensor_db_manager) | **3탭 센서 모니터링 및 SQLite 로깅 시스템** | 비동기 TCP 소켓 패킷 파싱 및 터치용 온스크린 키보드(`Tab1_Socket`), 3채널 실시간 시계열 차트(`Tab2_Sensor`), SQLite 기간별 조회·테이블/차트 동기화(`Tab3_Sqlite`) |
| [`0710_aiot_control_client`](./0710_aiot_control_client) | **7탭 통합 임베디드 제어·모니터링 HMI** | `/dev/ledkey` 커널 드라이버 연동(`Tab1`), TCP 소켓 라우팅·가상 키보드(`Tab2`), 원격 액추에이터 제어판(`Tab3`), 실시간 차트(`Tab4`), SQLite DB(`Tab5`), `QProcess` 카메라 제어(`Tab6`), `QThread`+OpenCV 비전 자동 제어(`Tab7`) |

---

## 3. 빌드 및 실행 환경

- **Host 환경**: Fedora Linux (Qt 6.x, GCC, qmake / CMake, OpenCV 4, SQLite 3)
- **Target 환경**: Raspberry Pi 4B (Embedded Linux, Qt 5.15, `/dev/ledkey` 문자 디바이스 드라이버, 터치스크린 디스플레이) 및 STM32F411RE (UART 115200bps)
- **Qt5 / Qt6 호환성**: `#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)` 매크로를 통해 호스트(Qt6)와 타겟 보드(Qt5)에서 동일한 소스 코드로 빌드 가능하도록 구성.

### 빌드 방법

```bash
# qmake 기반 모듈 (0702, 0703, 0709, 0710)
cd 0710_aiot_control_client
qmake AIoTClient.pro
make -j$(nproc)

# CMake 기반 모듈 (0421)
cd 0421_stm32_serial_monitor
cmake -B build
cmake --build build -j$(nproc)
```
