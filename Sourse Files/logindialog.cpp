#include "logindialog.h"
#include "userstore.h"
#include "titlebar.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QScreen>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/icons/app_icon.png"));

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect geo = screen->availableGeometry();
    resize(geo.width() / 2, geo.height() / 2);

    UserStore::ensureDefaultAdmin();
    setupUi();
}

LoginDialog::~LoginDialog() {}

QLabel* LoginDialog::buildLogo()
{
    QLabel *logo = new QLabel;
    logo->setPixmap(QPixmap(":/icons/app_icon.png").scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setAlignment(Qt::AlignCenter);
    return logo;
}

void LoginDialog::setupUi()
{
    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(20, 20, 20, 20);

    auto *card = new QWidget;
    card->setObjectName("card");
    card->setStyleSheet("QWidget#card { background-color: #ffffff; border-radius: 16px; border: 1px solid #e0e0e0; }");

    auto *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(40);
    shadow->setOffset(0, 8);
    shadow->setColor(QColor(0, 0, 0, 90));
    card->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(0);

    auto *titleBar = new TitleBar("CYBERLOG");
    connect(titleBar, &TitleBar::minimizeClicked, this, &LoginDialog::animatedMinimize);
    connect(titleBar, &TitleBar::closeClicked, this, &QDialog::reject);

    stack = new AnimatedStackedWidget;
    stack->addWidget(buildLoginPage());
    stack->addWidget(buildRegisterPage());

    cardLayout->addWidget(titleBar);
    cardLayout->addWidget(stack);
    outer->addWidget(card);
}

QWidget* LoginDialog::buildLoginPage()
{
    QWidget *page = new QWidget;
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(48, 24, 48, 40);
    layout->setSpacing(14);

    layout->addWidget(buildLogo());
    layout->addSpacing(8);

    auto *subtitle = new QLabel("Вхід у систему аналізу логів");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #6e6e6e; font-size: 12px; letter-spacing: 1px; background: transparent;");

    auto *userLabel = new QLabel("Логін");
    userLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600; letter-spacing: 1px; background: transparent;");
    loginUserInput = new AnimatedLineEdit;

    auto *passLabel = new QLabel("Пароль");
    passLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600; letter-spacing: 1px; background: transparent;");
    loginPassInput = new AnimatedLineEdit;
    loginPassInput->setEchoMode(QLineEdit::Password);

    loginButton = new AnimatedButton("УВІЙТИ");
    loginButton->setFixedHeight(42);
    loginButton->setColors(QColor("#2b2b2b"), QColor("#3f3f3f"), QColor("#171717"), QColor("#ffffff"));

    goToRegisterButton = new AnimatedButton("Немає акаунту? Зареєструватись");
    goToRegisterButton->setFlat(true);
    goToRegisterButton->setFixedHeight(30);
    goToRegisterButton->setColors(QColor(0,0,0,0), QColor(0,0,0,0), QColor(0,0,0,0), QColor("#6e6e6e"));

    layout->addWidget(subtitle);
    layout->addSpacing(16);
    layout->addWidget(userLabel);
    layout->addWidget(loginUserInput);
    layout->addWidget(passLabel);
    layout->addWidget(loginPassInput);
    layout->addSpacing(10);
    layout->addWidget(loginButton);
    layout->addWidget(goToRegisterButton, 0, Qt::AlignHCenter);
    layout->addStretch();

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(goToRegisterButton, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
    return page;
}

QWidget* LoginDialog::buildRegisterPage()
{
    QWidget *page = new QWidget;
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(48, 24, 48, 40);
    layout->setSpacing(14);

    layout->addWidget(buildLogo());
    layout->addSpacing(8);

    auto *subtitle = new QLabel("Новий обліковий запис аналітика");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #6e6e6e; font-size: 12px; letter-spacing: 1px; background: transparent;");

    auto *userLabel = new QLabel("Логін");
    userLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600; letter-spacing: 1px; background: transparent;");
    regUserInput = new AnimatedLineEdit;

    auto *passLabel = new QLabel("Пароль");
    passLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600; letter-spacing: 1px; background: transparent;");
    regPassInput = new AnimatedLineEdit;
    regPassInput->setEchoMode(QLineEdit::Password);

    auto *confirmLabel = new QLabel("Підтвердження пароля");
    confirmLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600; letter-spacing: 1px; background: transparent;");
    regPassConfirmInput = new AnimatedLineEdit;
    regPassConfirmInput->setEchoMode(QLineEdit::Password);

    registerButton = new AnimatedButton("СТВОРИТИ АКАУНТ");
    registerButton->setFixedHeight(42);
    registerButton->setColors(QColor("#2b2b2b"), QColor("#3f3f3f"), QColor("#171717"), QColor("#ffffff"));

    goToLoginButton = new AnimatedButton("Вже є акаунт? Увійти");
    goToLoginButton->setFlat(true);
    goToLoginButton->setFixedHeight(30);
    goToLoginButton->setColors(QColor(0,0,0,0), QColor(0,0,0,0), QColor(0,0,0,0), QColor("#6e6e6e"));

    layout->addWidget(subtitle);
    layout->addSpacing(16);
    layout->addWidget(userLabel);
    layout->addWidget(regUserInput);
    layout->addWidget(passLabel);
    layout->addWidget(regPassInput);
    layout->addWidget(confirmLabel);
    layout->addWidget(regPassConfirmInput);
    layout->addSpacing(10);
    layout->addWidget(registerButton);
    layout->addWidget(goToLoginButton, 0, Qt::AlignHCenter);
    layout->addStretch();

    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::handleRegister);
    connect(goToLoginButton, &QPushButton::clicked, this, &LoginDialog::switchToLogin);
    return page;
}

void LoginDialog::switchToRegister() { stack->setCurrentIndexAnimated(1); }
void LoginDialog::switchToLogin()    { stack->setCurrentIndexAnimated(0); }

void LoginDialog::handleLogin()
{
    QString user = loginUserInput->text().trimmed();
    QString pass = loginPassInput->text().trimmed();
    QMap<QString, QString> users;
    UserStore::loadUsers(users);
    if (users.contains(user) && users.value(user) == pass) {
        m_loggedInUser = user;
        accept();
    } else {
        QMessageBox::warning(this, "Помилка", "Невірний логін або пароль.");
    }
}

void LoginDialog::handleRegister()
{
    QString user = regUserInput->text().trimmed();
    QString pass = regPassInput->text().trimmed();
    QString confirm = regPassConfirmInput->text().trimmed();

    if (user.isEmpty() || pass.isEmpty()) { QMessageBox::warning(this, "Помилка", "Заповніть усі поля."); return; }
    if (pass != confirm) { QMessageBox::warning(this, "Помилка", "Паролі не співпадають."); return; }

    QMap<QString, QString> users;
    UserStore::loadUsers(users);
    if (users.contains(user)) { QMessageBox::warning(this, "Помилка", "Такий логін вже зайнятий."); return; }

    UserStore::saveUser(user, pass);
    QMessageBox::information(this, "Готово", "Акаунт створено. Тепер увійдіть.");
    regUserInput->clear(); regPassInput->clear(); regPassConfirmInput->clear();
    switchToLogin();
}

void LoginDialog::animatedMinimize()
{
    auto *anim = new QPropertyAnimation(this, "windowOpacity", this);
    anim->setDuration(150);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InCubic);
    connect(anim, &QPropertyAnimation::finished, this, [this]() {
        showMinimized();
        setWindowOpacity(1.0);
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}