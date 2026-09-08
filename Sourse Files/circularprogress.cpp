#include "circularprogress.h"
#include <QPainter>

CircularProgress::CircularProgress(QWidget *parent) : QWidget(parent)
{
    m_anim = new QPropertyAnimation(this, "animProgress", this);
    m_anim->setDuration(900);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);
}

void CircularProgress::setSegments(const QVector<ChartSegment> &segments)
{
    m_segments = segments;
    m_anim->stop();
    m_anim->setStartValue(0.0);
    m_anim->setEndValue(1.0);
    m_anim->start();
}

void CircularProgress::setCenterText(const QString &title, const QString &value)
{
    m_centerTitle = title;
    m_centerValue = value;
    update();
}

void CircularProgress::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    QRectF rect((width() - side) / 2.0 + 12, (height() - side) / 2.0 + 12, side - 24, side - 24);

    p.setPen(QPen(QColor("#e2e2e2"), 18, Qt::SolidLine, Qt::FlatCap));
    p.drawArc(rect, 0, 360 * 16);

    double startAngle = 90 * 16;
    for (const ChartSegment &seg : m_segments) {
        double sweep = -(seg.percent / 100.0) * 360.0 * 16.0 * m_animProgress;
        p.setPen(QPen(seg.color, 18, Qt::SolidLine, Qt::FlatCap));
        p.drawArc(rect, startAngle, sweep);
        startAngle += sweep;
    }

    p.setPen(QColor("#1c1c1c"));
    QFont f = font();
    f.setPointSize(20);
    f.setBold(true);
    p.setFont(f);
    p.drawText(rect, Qt::AlignCenter, m_centerValue);

    QFont f2 = font();
    f2.setPointSize(9);
    p.setFont(f2);
    p.setPen(QColor("#6e6e6e"));
    p.drawText(rect.adjusted(0, 30, 0, 30), Qt::AlignCenter, m_centerTitle);
}