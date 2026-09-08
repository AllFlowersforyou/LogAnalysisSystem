#ifndef TITLEBAR_H
#define TITLEBAR_H
#include <QWidget>
#include "animatedbutton.h"

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(const QString &title, QWidget *parent = nullptr);
signals:
    void minimizeClicked();
    void closeClicked();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QPoint m_dragPos;
    bool m_dragging = false;
};
#endif