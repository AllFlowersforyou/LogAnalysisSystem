#include "animatedstackedwidget.h"
#include <QLabel>

AnimatedStackedWidget::AnimatedStackedWidget(QWidget *parent) : QStackedWidget(parent) {}

void AnimatedStackedWidget::setCurrentIndexAnimated(int index)
{
    if (m_animating || index == currentIndex() || index < 0 || index >= count()) return;
    m_animating = true;

    QWidget *oldWidget = currentWidget();
    QPixmap oldSnapshot = oldWidget->grab();

    setCurrentIndex(index);
    QWidget *newWidget = currentWidget();
    QRect geo = newWidget->geometry();

    auto *oldOverlay = new QLabel(this);
    oldOverlay->setPixmap(oldSnapshot);
    oldOverlay->setGeometry(geo);
    oldOverlay->show();
    oldOverlay->raise();

    auto *newEffect = new QGraphicsOpacityEffect(newWidget);
    newWidget->setGraphicsEffect(newEffect);
    newEffect->setOpacity(0.0);

    auto *oldEffect = new QGraphicsOpacityEffect(oldOverlay);
    oldOverlay->setGraphicsEffect(oldEffect);

    auto *anim = new QParallelAnimationGroup(this);

    auto *fadeOut = new QPropertyAnimation(oldEffect, "opacity");
    fadeOut->setDuration(220);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::InOutCubic);

    auto *fadeIn = new QPropertyAnimation(newEffect, "opacity");
    fadeIn->setDuration(220);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutCubic);

    anim->addAnimation(fadeOut);
    anim->addAnimation(fadeIn);

    connect(anim, &QParallelAnimationGroup::finished, this, [this, oldOverlay, newWidget]() {
        oldOverlay->deleteLater();
        newWidget->setGraphicsEffect(nullptr);
        m_animating = false;
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}