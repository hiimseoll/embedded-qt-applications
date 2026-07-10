# 0702_qt_vi_editor (C/C++ 구문 강조 코드 뷰어)

## 1. 구현 개요
`QSplitter`를 활용해 좌측에는 `QTreeView`와 `QFileSystemModel` 기반의 디렉터리 탐색기를 배치하고, 우측에는 줄 번호 여백(`LineNumberArea`)과 정규표현식 기반 C/C++ 구문 강조(`QSyntaxHighlighter`)가 적용된 커스텀 코드 뷰어(`CodeEditor`)를 구현함.

## 2. 주요 구현 디테일 및 핵심 코드
```cpp
void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    // 다중 행 주석(/* ... */) 블록 상태 추적 및 포맷 적용
}
```
- **스크롤 연동 줄 번호 여백(`LineNumberArea`)**: `QPlainTextEdit`의 `blockCountChanged` 및 `updateRequest` 시그널에 슬롯을 연결하여, 문서 줄 수에 맞춰 좌측 여백 폭(`setViewportMargins`)을 동적으로 계산하고 현재 화면에 보이는 텍스트 블록의 줄 번호만 `QPainter`로 직접 렌더링함.
- **상태 기반 다중 행 주석 하이라이팅**: 단일 행 정규표현식 매칭(C/C++ 예약어, 클래스, 문자열, `//` 주석) 외에, 여러 줄에 걸친 `/* ... */` 주석을 처리하기 위해 `previousBlockState()`와 `setCurrentBlockState(1)`를 활용한 블록 간 상태 전이 로직을 구현함.

## 3. 기술적 한계 및 해결
하드코딩된 로컬 경로 대신 실행 환경에 구애받지 않도록 `QDir::homePath()`를 기준으로 파일 시스템 모델을 초기화하였으며, `QTreeView`에서 파일명 외의 불필요한 메타데이터 컬럼(크기, 타입, 수정일)을 숨겨 코드 탐색에 집중할 수 있는 레이아웃을 구성함.
