# 0703_webcam_stream_viewer (4채널 MJPEG 스트리밍 뷰어)

## 1. 구현 개요
다수의 임베디드 카메라 노드에서 송출하는 HTTP MJPEG 영상 스트림(`mjpg_streamer`)을 한 화면에서 동시에 관제할 수 있도록, 4개의 `QWebEngineView` 인스턴스를 2×2 그리드 레이아웃으로 배치한 멀티 채널 스트리밍 뷰어를 구현함.

## 2. 주요 구현 디테일 및 핵심 코드
```cpp
p_QWebEngineView0_0 = new QWebEngineView(this);
p_QWebEngineView0_0->load(QUrl("http://127.0.0.1:8080/?action=stream"));
ui->verticalLayout0_0->addWidget(p_QWebEngineView0_0);
```
- 각 채널별 컨테이너 레이아웃(`verticalLayout0_0` ~ `1_1`)에 독립된 `QWebEngineView` 위젯을 동적 생성하여 바인딩하고, 각 카메라 스트리밍 서버의 `/?action=stream` 엔드포인트를 비동기 로드함.
- 창 크기 조절 시 4개 채널 뷰어가 균등한 비율로 리사이징되도록 레이아웃 제약을 구성함.

## 3. 기술적 한계 및 해결
Linux(Wayland) 및 소수점 디스플레이 배율 환경에서 Chromium 기반 `QWebEngineView` 초기화 시 OpenGL 컨텍스트 재생성으로 인해 화면 깜빡임이나 투명화 현상이 발생할 수 있음을 확인하고, 플랫폼 백엔드(`QT_QPA_PLATFORM=xcb`) 및 렌더링 컨텍스트 설정을 통해 안정적인 멀티 스트림 출력을 검증함.
