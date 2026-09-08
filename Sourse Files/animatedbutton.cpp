#include "animatedbutton.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

AnimatedButton::AnimatedButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
    m_normalColor = QColor("#2b2b2b");
    m_hoverColor  = QColor("#3f3f3f");
    m_pressedColor= QColor("#171717");
    m_textColor   = QColor("#ffffff");
    m_bgColor = m_normalColor;

    m_anim = new QPropertyAnimation(this, "bgColor", this);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);
}

void AnimatedButton::setColors(const QColor &normal, const QColor &hover, const QColor &pressed, const QColor &textColor)
{
    m_normalColor = normal; m_hoverColor = hover;
    m_pressedColor = pressed; m_textColor = textColor;
    m_bgColor = normal;
    update();
}

void AnimatedButton::animateTo(const QColor &target, int duration)
{
    m_anim->stop();
    m_anim->setDuration(duration);
    m_anim->setStartValue(m_bgColor);
    m_anim->setEndValue(target);
    m_anim->start();
}

void AnimatedButton::enterEvent(QEnterEvent *event) { animateTo(m_hoverColor); QPushButton::enterEvent(event); }
void AnimatedButton::leaveEvent(QEvent *event) { animateTo(m_normalColor); QPushButton::leaveEvent(event); }
void AnimatedButton::mousePressEvent(QMouseEvent *event) { animateTo(m_pressedColor, 80); QPushButton::mousePressEvent(event); }
void AnimatedButton::mouseReleaseEvent(QMouseEvent *event)
{
    animateTo(rect().contains(event->pos()) ? m_hoverColor : m_normalColor, 120);
    QPushButton::mouseReleaseEvent(event);
}

void AnimatedButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), 6, 6);
    p.fillPath(path, m_bgColor);
    p.setPen(m_textColor);
    p.setFont(font());
    p.drawText(rect(), Qt::AlignCenter, text());
}