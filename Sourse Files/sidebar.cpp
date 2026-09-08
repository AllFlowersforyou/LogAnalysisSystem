#include "sidebar.h"
#include "navbutton.h"
#include <QVBoxLayout>

Sidebar::Sidebar(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(210);
    setStyleSheet("background-color: #eeeeee;");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setSpacing(4);

    struct Item { NavIcon icon; QString text; };
    QVector<Item> items = {
        {NavIcon::Home, "Головна"},
        {NavIcon::List, "Методи парсингу"},
        {NavIcon::User, "Аккаунт"}
    };

    for (int i = 0; i < items.size(); ++i) {
        auto *btn = new NavButton(items[i].icon, items[i].text);
        m_buttons.append(btn);
        layout->addWidget(btn);
        connect(btn, &NavButton::clicked, this, [this, i]() { emit navSelected(i); });
    }
    m_buttons.first()->setActive(true);
    layout->addStretch();
}

void Sidebar::setCurrentIndex(int index)
{
    for (int i = 0; i < m_buttons.size(); ++i) m_buttons[i]->setActive(i == index);
}