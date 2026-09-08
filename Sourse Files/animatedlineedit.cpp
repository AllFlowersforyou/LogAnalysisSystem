#include "animatedlineedit.h"
#include <QPainter>

AnimatedLineEdit::AnimatedLineEdit(QWidget *parent) : QLineEdit(parent)
{
    m_lineColor = m_idleColor;
    setStyleSheet("QLineEdit { background: transparent; border: none; padding: 8px 2px; color: #1c1c1c; font-size: 14px; }");
    m_anim = new QPropertyAnimation(this, "lineColor", this);
    m_anim->setDuration(200);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);
}

void AnimatedLineEdit::focusInEvent(QFocusEvent *event)
{
    m_anim->stop(); m_anim->setStartValue(m_lineColor); m_anim->setEndValue(m_activeColor); m_anim->start();
    QLineEdit::focusInEvent(event);
}
void AnimatedLineEdit::focusOutEvent(QFocusEvent *event)
{
    m_anim->stop(); m_anim->setStartValue(m_lineColor); m_anim->setEndValue(m_idleColor); m_anim->start();
    QLineEdit::focusOutEvent(event);
}
void AnimatedLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(m_lineColor, 1.5));
    p.drawLine(0, height() - 2, width(), height() - 2);
}