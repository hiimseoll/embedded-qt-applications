# 0709_sensor_db_manager (3탭 센서 모니터링 및 SQLite 로깅 시스템)

## 1. 구현 개요
TCP 소켓 서버로부터 실시간 환경 센서 데이터(`[송신자]SENSOR@조도@온도@습도`)를 수신하여 파싱하고, 이를 실시간 3채널 시계열 그래프(`Qt Charts`)와 로컬 관계형 데이터베이스(`SQLite`)에 동시 반영하며 기간별 이력 조회·시각화를 지원하는 3탭 구조의 모니터링 애플리케이션(`SensorManSDH`)을 구현함.

## 2. 주요 구현 디테일 및 핵심 코드
```cpp
// mainwidget.cpp: 상위 컨테이너에서 탭 간 데이터 시그널 중계
connect(p_Tab1_Socket, SIGNAL(tab2RecvDataSig(QString)),
        p_Tab2_Sensor, SLOT(Tab2RecvDataSlot(QString)));
connect(p_Tab1_Socket, SIGNAL(tab3RecvDataSig(QString)),
        p_Tab3_Sqlite, SLOT(Tab3RecvDataSlot(QString)));
```
- **Tab 1 (`Tab1_Socket`)**: `QTcpSocket`의 `readyRead()` 시그널을 통해 수신된 패킷을 `@` 구분자로 파싱하고, `SENSOR` 명령일 경우 상위 `mainWidget`을 통해 Tab 2와 Tab 3으로 데이터를 중계함. 물리 키보드가 없는 임베디드 터치 패널 환경을 위해 포커스된 `QLineEdit` 포인터(`sender()`)에 문자를 주입하는 가상 키보드(`Keyboard`)를 연동함.
- **Tab 2 (`Tab2_Sensor`)**: 조도·온도·습도 3개의 `QLineSeries`와 `QDateTimeAxis("hh:mm")`를 결합하여 수신 시각(`toMSecsSinceEpoch()`) 기준 실시간 다중 꺾은선 차트를 렌더링하고 X축 시간 범위를 동적으로 갱신함.
- **Tab 3 (`Tab3_Sqlite`)**: `QSqlDatabase::addDatabase("QSQLITE")`로 로컬 `aiot.db`의 `sensor_tb` 테이블에 데이터를 실시간 `INSERT`하고, `QDateTimeEdit`으로 지정한 시작~종료 구간의 레코드를 `SELECT`하여 좌측 `QTableWidget`과 우측 `QChartView`에 동시에 복원함.

## 3. 기술적 한계 및 해결
DB 조회 시 테이블(`QTableWidget`)에는 데이터가 정상 출력되나 차트(`QChartView`)에 그래프가 그려지지 않는 문제를 디버깅함. 분석 결과 시간축 범위 갱신 함수 내부에서 클래스 멤버 변수(`firstDateTime`, `lastDateTime`)와 동일한 이름의 지역 변수를 재선언하는 **변수 Shadowing(가리기) 버그**로 인해 정작 `axisX->setRange()`에 전달되는 멤버 변수가 초기값에 머물러 있었음을 확인하고, 지역 변수 선언을 제거해 해결함.
