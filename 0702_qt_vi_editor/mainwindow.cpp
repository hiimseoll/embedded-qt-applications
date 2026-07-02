#include "mainwindow.h"
#include <QHeaderView>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QPainter>
#include <QTextBlock>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupStyles();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle(tr("Adwaita Code Viewer"));
    resize(1000, 650);

    splitter = new QSplitter(Qt::Horizontal, this);

    // 파일 탐색기 설정
    fileTreeView = new QTreeView(splitter);
    fileSystemModel = new QFileSystemModel(this);
    
    // 루트 경로 설정
    QString rootPath = QDir::homePath();
    fileSystemModel->setRootPath(rootPath);
    fileTreeView->setModel(fileSystemModel);
    fileTreeView->setRootIndex(fileSystemModel->index(rootPath));

    fileTreeView->hideColumn(1);
    fileTreeView->hideColumn(2);
    fileTreeView->hideColumn(3);
    fileTreeView->header()->hide();

    // 코드 에디터 설정
    codeEditor = new CodeEditor(splitter);
    codeEditor->setReadOnly(true);
    highlighter = new Highlighter(codeEditor->document());

    splitter->addWidget(fileTreeView);
    splitter->addWidget(codeEditor);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    setCentralWidget(splitter);

    connect(fileTreeView, &QTreeView::clicked, this, &MainWindow::onFileSelected);
}

void MainWindow::onFileSelected(const QModelIndex &index)
{
    QString filePath = fileSystemModel->filePath(index);
    QFileInfo fileInfo(filePath);

    if (fileInfo.isFile()) {
        QFile file(filePath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            codeEditor->setPlainText(in.readAll());
            file.close();
        }
    }
}

void MainWindow::setupStyles()
{
    // Libadwaita Dark 규정을 반영한 QSS 스타일시트 설정
    QString styleSheet = R"(
        QMainWindow {
            background-color: #1e1e1e;
        }
        QSplitter::handle {
            background-color: #242424;
            width: 1px;
        }
        QTreeView {
            background-color: #1e1e1e;
            color: #e0e0e0;
            border: none;
            border-right: 1px solid #282828;
            padding: 12px;
            font-family: "Cascadia Code", "Segoe UI", sans-serif;
            font-weight: normal;
            font-size: 13px;
            show-decoration-selected: 1;
        }
        QTreeView::item {
            padding: 4px 8px;
            border-radius: 4px;
            margin-bottom: 2px;
        }
        QTreeView::item:hover {
            background-color: #2c2c2c;
        }
        QTreeView::item:selected {
            background-color: #303030;
            color: #3584e4;
            font-weight: bold;
        }
        QTreeView::branch {
            background: transparent;
        }
        QTreeView::branch:has-children:!has-siblings:closed,
        QTreeView::branch:closed:has-children:has-siblings {
            border-image: none;
            image: url(none);
        }
        QPlainTextEdit {
            background-color: #1e1e1e;
            color: #f8f8f2;
            border: none;
            font-family: "Cascadia Code", monospace;
            font-weight: normal;
            font-size: 13px;
        }
    )";
    setStyleSheet(styleSheet);
}

// --- CodeEditor Implementation ---

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        digits++;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    int space = 20 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#else
    int space = 20 + fontMetrics().width(QLatin1Char('9')) * digits;
#endif
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor("#2a2a2a");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor("#1e1e1e"));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("#6e7681"));
            painter.drawText(0, top, lineNumberArea->width() - 8, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        blockNumber++;
    }
}
