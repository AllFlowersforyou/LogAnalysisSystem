#ifndef ANIMATEDSTACKEDWIDGET_H
#define ANIMATEDSTACKEDWIDGET_H
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>

class AnimatedStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AnimatedStackedWidget(QWidget *parent = nullptr);
    void setCurrentIndexAnimated(int index);
private:
    bool m_animating = false;
};
#endif