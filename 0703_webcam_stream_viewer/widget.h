#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebEngineView>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    Ui::Widget *ui;
    QWebEngineView* p_QWebEngineView0_0;
    QWebEngineView* p_QWebEngineView0_1;
    QWebEngineView* p_QWebEngineView1_0;
    QWebEngineView* p_QWebEngineView1_1;
};
#endif // WIDGET_H
