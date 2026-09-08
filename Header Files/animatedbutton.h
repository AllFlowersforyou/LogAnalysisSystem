#ifndef ANIMATEDBUTTON_H
#define ANIMATEDBUTTON_H
#include <QPushButton>
#include <QPropertyAnimation>
#include <QColor>

class AnimatedButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor)
public:
    explicit AnimatedButton(const QString &text, QWidget *parent = nullptr);
    void setColors(const QColor &normal, const QColor &hover, const QColor &pressed, const QColor &textColor);

    QColor bgColor() const { return m_bgColor; }
    void setBgColor(const QColor &c) { m_bgColor = c; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QColor m_bgColor, m_normalColor, m_hoverColor, m_pressedColor, m_textColor;
    QPropertyAnimation *m_anim;
    void animateTo(const QColor &target, int duration = 180);
};
#endif