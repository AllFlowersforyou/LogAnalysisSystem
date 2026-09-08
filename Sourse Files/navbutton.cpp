#include "navbutton.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

NavButton::NavButton(NavIcon icon, const QString &text, QWidget *parent)
    : QWidget(parent), m_icon(icon), m_text(text)
{
    setFixedHeight(46);
    setCursor(Qt::PointingHandCursor);
    m_anim = new QPropertyAnimation(this, "bgColor", this);
    m_anim->setDuration(180);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);
}

void NavButton::setActive(bool active) { m_active = active; update(); }

void NavButton::enterEvent(QEnterEvent *event)
{
    if (!m_active) {
        m_anim->stop();
        m_anim->setStartValue(m_bgColor);
        m_anim->setEndValue(QColor(0, 0, 0, 18));
        m_anim->start();
    }
    QWidget::enterEvent(event);
}
void NavButton::leaveEvent(QEvent *event)
{
    if (!m_active) {
        m_anim->stop();
        m_anim->setStartValue(m_bgColor);
        m_anim->setEndValue(QColor(0, 0, 0, 0));
        m_anim->start();
    }
    QWidget::leaveEvent(event);
}
void NavButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (rect().contains(event->pos())) emit clicked();
    QWidget::mouseReleaseEvent(event);
}

void NavButton::drawIcon(QPainter &p, const QRectF &r, const QColor &color)
{
    p.setPen(QPen(color, 1.6));
    p.setBrush(Qt::NoBrush);
    switch (m_icon) {
    case NavIcon::Home: {
        QPainterPath path;
        path.moveTo(r.left(), r.center().y());
        path.lineTo(r.center().x(), r.top());
        path.lineTo(r.right(), r.center().y());
        p.drawPath(path);
        p.drawRect(QRectF(r.left() + r.width() * 0.2, r.center().y(), r.width() * 0.6, r.height() * 0.4));
        break;
    }
    case NavIcon::List:
        for (int i = 0; i < 3; ++i) {
            double y = r.top() + i * (r.height() / 2.4);
            p.drawLine(QPointF(r.left(), y), QPointF(r.right(), y));
        }
        break;
    case NavIcon::User: {
        double d = r.width() * 0.5;
        p.drawEllipse(QPointF(r.center().x(), r.top() + d * 0.5), d * 0.5, d * 0.5);
        QPainterPath body;
        body.moveTo(r.left(), r.bottom());
        body.arcTo(QRectF(r.left(), r.bottom() - r.width() * 0.9, r.width(), r.width()), 180, 180);
        p.drawPath(body);
        break;
    }
    }
}

void NavButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QColor bg = m_active ? QColor(0, 0, 0, 10) : m_bgColor;
    p.fillRect(rect(), bg);
    if (m_active) p.fillRect(QRectF(0, 6, 3, height() - 12), QColor("#1c1c1c"));

    QColor fg = m_active ? QColor("#1c1c1c") : QColor("#7a7a7a");
    drawIcon(p, QRectF(18, height() / 2.0 - 9, 18, 18), fg);

    p.setPen(fg);
    QFont f = font();
    f.setPointSize(10);
    f.setBold(m_active);
    p.setFont(f);
    p.drawText(QRectF(50, 0, width() - 60, height()), Qt::AlignVCenter | Qt::AlignLeft, m_text);
}