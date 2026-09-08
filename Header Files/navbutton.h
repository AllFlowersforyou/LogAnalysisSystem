#ifndef NAVBUTTON_H
#define NAVBUTTON_H
#include <QWidget>
#include <QPropertyAnimation>

enum class NavIcon { Home, List, User };

class NavButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor)
public:
    explicit NavButton(NavIcon icon, const QString &text, QWidget *parent = nullptr);
    void setActive(bool active);
    QColor bgColor() const { return m_bgColor; }
    void setBgColor(const QColor &c) { m_bgColor = c; update(); }
signals:
    void clicked();
protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    NavIcon m_icon;
    QString m_text;
    bool m_active = false;
    QColor m_bgColor{0, 0, 0, 0};
    QPropertyAnimation *m_anim;
    void drawIcon(QPainter &p, const QRectF &r, const QColor &color);
};
#endif