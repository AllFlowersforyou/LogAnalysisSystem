#ifndef ANIMATEDLINEEDIT_H
#define ANIMATEDLINEEDIT_H
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QColor>

class AnimatedLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
public:
    explicit AnimatedLineEdit(QWidget *parent = nullptr);
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor &c) { m_lineColor = c; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QColor m_lineColor;
    QColor m_idleColor{"#c9c9c9"};
    QColor m_activeColor{"#1c1c1c"};
    QPropertyAnimation *m_anim;
};
#endif