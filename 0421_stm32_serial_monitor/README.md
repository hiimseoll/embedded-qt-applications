# 0421_stm32_serial_monitor (STM32 UART CLI 제어 및 센서 모니터)

## 1. 구현 개요
STM32F411 보드와 UART 시리얼(`QSerialPort`, `/dev/ttyACM*`, 115200bps 8N1)로 연결하여 CLI 제어 명령(`monitor on`, `temp`, `led toggle`)을 송신하고, 보드에서 주기적으로 올라오는 센서·상태 패킷을 비동기로 파싱해 UI에 표시하는 제어·모니터링 애플리케이션을 구현함.

## 2. 주요 구현 디테일 및 핵심 코드
```cpp
void MainWindow::readData(){
    m_buffer.append(serial->readAll());

    while(m_buffer.contains('$') && m_buffer.contains('#')){
        int start = m_buffer.indexOf('$');
        int end = m_buffer.indexOf('#', start);
        if(end == -1) break;

        QByteArray packet = m_buffer.mid(start + 1, end - start - 1);
        m_buffer.remove(0, end + 1);

        if(!packet.isEmpty()){
            parse_protocol(packet);
        }
    }
}
```
- **바이트 스트림 누적 및 프레이밍 파서**: OS 시리얼 버퍼 특성상 패킷이 여러 번의 `readyRead` 시그널로 쪼개져 수신될 수 있으므로, 멤버 버퍼(`m_buffer`)에 바이트를 누적한 뒤 시작(`$`)과 종료(`#`) 구분자가 모두 확인될 때만 완전한 패킷(`count,sensor_id:type:val`)을 추출하여 파싱함.
- **포트 점유 프로세스 사전 검사**: 포트 오픈 전 `QProcess`로 `lsof -t <portPath>`를 실행하여 다른 터미널이나 프로세스가 시리얼 포트를 점유 중일 경우 해당 PID를 안내하고 충돌을 방지함.
- **동적 프로퍼티 기반 QSS 스타일 전환**: 연결 상태 변경 시 `setProperty("isConnected", true/false)`와 `style()->unpolish()` / `polish()`를 호출해 하드코딩된 인라인 스타일시트 없이 상태별 버튼 색상을 동적으로 갱신함.

## 3. 기술적 한계 및 해결
초기에는 `serial->readAll()`의 반환값을 줄바꿈(`\n`) 기준으로 즉시 분리했으나, 115200bps 연속 송신 시 패킷 중간이 잘려 파싱 오류가 발생하는 문제를 확인함. 이를 해결하기 위해 수신 버퍼(`m_buffer`)와 `$...#` 프레이밍 구조를 도입하여 조각난 바이트 스트림에서도 패킷 유실 없이 상태를 갱신하도록 개선함.
