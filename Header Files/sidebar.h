#ifndef SIDEBAR_H
#define SIDEBAR_H
#include <QWidget>
#include <QVector>
class NavButton;

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent = nullptr);
    void setCurrentIndex(int index);
signals:
    void navSelected(int index);
private:
    QVector<NavButton*> m_buttons;
};
#endif