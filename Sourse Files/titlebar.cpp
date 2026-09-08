#include "titlebar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

TitleBar::TitleBar(const QString &title, QWidget *parent) : QWidget(parent)
{
    setFixedHeight(40);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 0, 8, 0);
    layout->setSpacing(8);

    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/icons/app_icon.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; letter-spacing: 1px; font-weight: 600; background: transparent;");

    AnimatedButton *minBtn = new AnimatedButton("—");
    minBtn->setFixedSize(28, 28);
    minBtn->setColors(QColor("#eeeeee"), QColor("#dcdcdc"), QColor("#c9c9c9"), QColor("#1c1c1c"));

    AnimatedButton *closeBtn = new AnimatedButton("✕");
    closeBtn->setFixedSize(28, 28);
    closeBtn->setColors(QColor("#eeeeee"), QColor("#e53935"), QColor("#b71c1c"), QColor("#1c1c1c"));

    layout->addWidget(icon);
    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(minBtn);
    layout->addWidget(closeBtn);

    connect(minBtn, &QPushButton::clicked, this, &TitleBar::minimizeClicked);
    connect(closeBtn, &QPushButton::clicked, this, &TitleBar::closeClicked);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPos = event->globalPosition().toPoint() - window()->frameGeometry().topLeft();
    }
}
void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton))
        window()->move(event->globalPosition().toPoint() - m_dragPos);
}