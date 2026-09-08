#include "mainwindow.h"
#include "titlebar.h"
#include "sidebar.h"
#include "animatedbutton.h"
#include "userstore.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QRadioButton>
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QFrame>

MainWindow::MainWindow(const QString &currentUser, QWidget *parent)
    : QMainWindow(parent), m_currentUser(currentUser)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/icons/app_icon.png"));

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect geo = screen->availableGeometry();
    resize(geo.width() * 0.75, geo.height() * 0.8);

    setupUi();
}

MainWindow::~MainWindow() {}

QLabel* MainWindow::buildWatermark()
{
    auto *w = new QLabel("by Dogadkin");
    w->setStyleSheet("color: rgba(0,0,0,80); font-size: 10px; background: transparent;");
    return w;
}

void MainWindow::setupUi()
{
    auto *central = new QWidget;
    setCentralWidget(central);

    auto *outer = new QVBoxLayout(central);
    outer->setContentsMargins(16, 16, 16, 16);

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

    auto *titleBar = new TitleBar("CYBERLOG — АНАЛІЗ ЛОГІВ");
    connect(titleBar, &TitleBar::minimizeClicked, this, &MainWindow::animatedMinimize);
    connect(titleBar, &TitleBar::closeClicked, qApp, &QApplication::quit);

    auto *body = new QWidget;
    auto *bodyLayout = new QHBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    sidebar = new Sidebar;
    connect(sidebar, &Sidebar::navSelected, this, &MainWindow::onNavSelected);

    contentStack = new AnimatedStackedWidget;
    contentStack->addWidget(buildHomeTab());
    contentStack->addWidget(buildMethodsTab());
    contentStack->addWidget(buildAccountTab());

    auto *divider = new QFrame;
    divider->setFrameShape(QFrame::VLine);
    divider->setFixedWidth(1);
    divider->setStyleSheet("background-color: #1c1c1c; border: none;");

    bodyLayout->addWidget(sidebar);
    bodyLayout->addWidget(divider);
    bodyLayout->addWidget(contentStack, 1);

    cardLayout->addWidget(titleBar);
    cardLayout->addWidget(body, 1);

    auto *watermarkRow = new QHBoxLayout;
    watermarkRow->setContentsMargins(0, 0, 12, 8);
    watermarkRow->addStretch();
    watermarkRow->addWidget(buildWatermark());
    cardLayout->addLayout(watermarkRow);

    outer->addWidget(card);
}

void MainWindow::onNavSelected(int index)
{
    if (m_navLocked) return;
    m_navLocked = true;
    sidebar->setEnabled(false);

    QTimer::singleShot(90, this, [this, index]() {
        sidebar->setCurrentIndex(index);
        contentStack->setCurrentIndexAnimated(index);
        sidebar->setEnabled(true);
        m_navLocked = false;
    });
}

void MainWindow::animatedMinimize()
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

QColor MainWindow::colorForSeverity(const QString &sev) const
{
    if (sev == "CRITICAL") return QColor("#e53935");
    if (sev == "ERROR") return QColor("#ff8a65");
    if (sev == "WARNING") return QColor("#fbc02d");
    return QColor("#9e9e9e"); // INFO / невідомо
}

QWidget* MainWindow::buildHomeTab()
{
    auto *page = new QWidget;
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(36, 32, 36, 24);
    layout->setSpacing(20);

    auto *headerRow = new QHBoxLayout;
    auto *logo = new QLabel;
    logo->setPixmap(QPixmap(":/icons/app_icon.png").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    auto *headerText = new QLabel("CyberLog — локальна система аналізу лог-файлів");
    headerText->setStyleSheet("color: #1c1c1c; font-size: 16px; font-weight: 700;");
    headerRow->addWidget(logo);
    headerRow->addWidget(headerText);
    headerRow->addStretch();

    auto *desc = new QLabel(
        "Завантажте текстовий файл логів, оберіть метод парсингу — програма знайде підозрілі "
        "події (Error, Denied тощо) та побудує статистику знайдених інцидентів.");
    desc->setWordWrap(true);
    desc->setStyleSheet("color: #6e6e6e; font-size: 12px;");

    auto *uploadBtn = new AnimatedButton("ЗАВАНТАЖИТИ ФАЙЛ ЛОГІВ (.txt / .json)");
    uploadBtn->setFixedHeight(44);
    uploadBtn->setColors(QColor("#2b2b2b"), QColor("#3f3f3f"), QColor("#171717"), QColor("#ffffff"));
    connect(uploadBtn, &QPushButton::clicked, this, &MainWindow::handleUploadFile);

    auto *methodLabel = new QLabel("МЕТОД ПАРСИНГУ");
    methodLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600; letter-spacing: 1px;");

    methodGroup = new QButtonGroup(page);
    auto *r1 = new QRadioButton("Базовий пошук за ключовими словами");
    auto *r2 = new QRadioButton("Класифікація за рівнем критичності");
    auto *r3 = new QRadioButton("Частотно-статистичний аналіз");
    r1->setChecked(true);
    methodGroup->addButton(r1, 0);
    methodGroup->addButton(r2, 1);
    methodGroup->addButton(r3, 2);
    for (auto *r : {r1, r2, r3})
        r->setStyleSheet("QRadioButton { color: #333333; font-size: 12px; padding: 4px; }");

    auto *splitRow = new QHBoxLayout;
    splitRow->setSpacing(24);

    resultsList = new QListWidget;
    resultsList->setStyleSheet(
        "QListWidget { background-color: #f7f7f7; border: 1px solid #dcdcdc; border-radius: 8px; color: #333333; font-size: 12px; }"
        "QListWidget::item { padding: 4px 10px; }");
    resultsList->addItem("Тут з'являться знайдені інциденти після завантаження файлу");

    auto *chartCol = new QVBoxLayout;
    resultChart = new CircularProgress;
    resultChart->setCenterText("ВСЬОГО РЯДКІВ", "—");

    auto *legendBox = new QVBoxLayout;
    legendBox->setSpacing(4);
    legendRed = new QLabel("● Критичні: —");
    legendYellow = new QLabel("● Попередження: —");
    legendGreen = new QLabel("● Інформаційні: —");
    legendRed->setStyleSheet("color: #e53935; font-size: 11px;");
    legendYellow->setStyleSheet("color: #fbc02d; font-size: 11px;");
    legendGreen->setStyleSheet("color: #43a047; font-size: 11px;");
    legendBox->addWidget(legendRed);
    legendBox->addWidget(legendYellow);
    legendBox->addWidget(legendGreen);

    chartCol->addWidget(resultChart, 0, Qt::AlignHCenter);
    chartCol->addLayout(legendBox);

    splitRow->addWidget(resultsList, 3);
    splitRow->addLayout(chartCol, 2);

    auto *saveRow = new QHBoxLayout;
    auto *saveLabel = new QLabel("Зберегти результат:");
    saveLabel->setStyleSheet("color: #4a4a4a; font-size: 11px; font-weight: 600;");
    auto *saveJsonBtn = new AnimatedButton("JSON");
    saveJsonBtn->setFixedSize(90, 32);
    saveJsonBtn->setColors(QColor("#2b2b2b"), QColor("#3f3f3f"), QColor("#171717"), QColor("#ffffff"));
    auto *saveTxtBtn = new AnimatedButton("TXT");
    saveTxtBtn->setFixedSize(90, 32);
    saveTxtBtn->setColors(QColor("#2b2b2b"), QColor("#3f3f3f"), QColor("#171717"), QColor("#ffffff"));
    connect(saveJsonBtn, &QPushButton::clicked, this, &MainWindow::handleSaveJson);
    connect(saveTxtBtn, &QPushButton::clicked, this, &MainWindow::handleSaveTxt);
    saveRow->addWidget(saveLabel);
    saveRow->addWidget(saveJsonBtn);
    saveRow->addWidget(saveTxtBtn);
    saveRow->addStretch();

    layout->addLayout(headerRow);
    layout->addWidget(desc);
    layout->addWidget(uploadBtn);
    layout->addSpacing(8);
    layout->addWidget(methodLabel);
    layout->addWidget(r1);
    layout->addWidget(r2);
    layout->addWidget(r3);
    layout->addSpacing(8);
    layout->addLayout(splitRow, 1);
    layout->addLayout(saveRow);

    return page;
}

QWidget* MainWindow::buildMethodsTab()
{
    auto *page = new QWidget;
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(36, 32, 36, 24);
    layout->setSpacing(16);

    auto *title = new QLabel("МЕТОДИ ПАРСИНГУ ЛОГІВ");
    title->setStyleSheet("color: #1c1c1c; font-size: 18px; font-weight: 700;");
    layout->addWidget(title);

    struct MethodInfo { QString name, desc; };
    QVector<MethodInfo> methods = {
        {"Базовий пошук за ключовими словами",
         "Найпростіший з трьох методів. Кожен рядок лог-файлу перевіряється регулярним виразом "
         "QRegularExpression на наявність ключових слів \"Error\", \"Denied\", \"Failed\" (без урахування "
         "регістру). У список результатів потрапляють лише ті рядки, де знайдено збіг — решта логу "
         "ігнорується. Метод швидкий і простий, але не розрізняє рівень серйозності події: попередження "
         "та критична помилка виглядають однаково, якщо містять одне з ключових слів."},
        {"Класифікація за рівнем критичності",
         "Аналізує КОЖЕН рядок логу (а не лише ті, де є ключові слова) і визначає його рівень за міткою "
         "у тексті: INFO, WARNING, ERROR або CRITICAL — за допомогою регулярного виразу з межами слова "
         "(\\b). Кожен рядок відображається у списку результатів з кольоровим кодуванням відповідно до "
         "рівня. Дозволяє побачити повну картину роботи системи, а не лише інциденти, і зрозуміти "
         "контекст навколо помилок."},
        {"Частотно-статистичний аналіз",
         "Не виводить окремі рядки, а підраховує кількість подій кожного рівня критичності за весь файл "
         "та обчислює їх відсоткове співвідношення відносно загальної кількості рядків. Саме цей метод "
         "живить кругову діаграму статистики: критичні та error-події формують червоний сегмент, "
         "warning — жовтий, info — зелений. Найкраще підходить для швидкої оцінки \"здоров'я\" системи "
         "без потреби вивчати кожен рядок окремо."}
    };

    for (const auto &m : methods) {
        auto *card = new QWidget;
        card->setStyleSheet("background-color: #f7f7f7; border: 1px solid #dcdcdc; border-radius: 8px;");
        auto *cl = new QVBoxLayout(card);
        auto *name = new QLabel(m.name);
        name->setStyleSheet("color: #1c1c1c; font-size: 13px; font-weight: 700; background: transparent; border: none;");
        auto *desc = new QLabel(m.desc);
        desc->setWordWrap(true);
        desc->setStyleSheet("color: #6e6e6e; font-size: 12px; background: transparent; border: none;");
        cl->addWidget(name);
        cl->addWidget(desc);
        layout->addWidget(card);
    }
    layout->addStretch();
    return page;
}

QWidget* MainWindow::buildAccountTab()
{
    auto *page = new QWidget;
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(36, 32, 36, 24);
    layout->setSpacing(14);

    auto *title = new QLabel("АККАУНТ");
    title->setStyleSheet("color: #1c1c1c; font-size: 18px; font-weight: 700;");

    auto *login = new QLabel("Логін: " + m_currentUser);
    login->setStyleSheet("color: #333333; font-size: 13px;");

    auto *changePassBtn = new AnimatedButton("ЗМІНИТИ ПАРОЛЬ");
    changePassBtn->setFixedHeight(40);
    changePassBtn->setFixedWidth(220);
    changePassBtn->setColors(QColor("#2b2b2b"), QColor("#3f3f3f"), QColor("#171717"), QColor("#ffffff"));
    connect(changePassBtn, &QPushButton::clicked, this, &MainWindow::handleChangePassword);

    auto *logoutBtn = new AnimatedButton("ВИЙТИ З АККАУНТА");
    logoutBtn->setFixedHeight(40);
    logoutBtn->setFixedWidth(220);
    logoutBtn->setColors(QColor("#f0f0f0"), QColor("#ffd6d6"), QColor("#ffb3b3"), QColor("#c62828"));
    connect(logoutBtn, &QPushButton::clicked, this, [this]() {
        emit loggedOut();
        close();
    });

    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(login);
    layout->addSpacing(10);
    layout->addWidget(changePassBtn);
    layout->addWidget(logoutBtn);
    layout->addStretch();
    return page;
}

void MainWindow::handleChangePassword()
{
    bool ok;
    QString oldPass = QInputDialog::getText(this, "Зміна пароля", "Поточний пароль:", QLineEdit::Password, "", &ok);
    if (!ok) return;

    QMap<QString, QString> users;
    UserStore::loadUsers(users);
    if (!users.contains(m_currentUser) || users.value(m_currentUser) != oldPass) {
        QMessageBox::warning(this, "Помилка", "Невірний поточний пароль.");
        return;
    }

    QString newPass = QInputDialog::getText(this, "Зміна пароля", "Новий пароль:", QLineEdit::Password, "", &ok);
    if (!ok || newPass.isEmpty()) return;

    UserStore::saveUser(m_currentUser, newPass);
    QMessageBox::information(this, "Готово", "Пароль успішно змінено.");
}

void MainWindow::handleUploadFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Оберіть файл логів", QString(), "Log files (*.txt *.json)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Помилка", "Не вдалося відкрити файл.");
        return;
    }
    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) lines << in.readLine();
    file.close();

    int method = methodGroup->checkedId();
    if (method < 0) method = 0;

    static const QStringList methodNames = {
        "Базовий пошук за ключовими словами",
        "Класифікація за рівнем критичності",
        "Частотно-статистичний аналіз"
    };

    resultsList->clear();
    m_lastIncidents.clear();

    static const QRegularExpression keywordRe("(Error|Denied|Failed)", QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression severityRe("\\b(INFO|WARNING|ERROR|CRITICAL)\\b");

    QMap<QString, int> severityCounts;

    for (const QString &line : lines) {
        if (line.trimmed().isEmpty()) continue;

        auto sevMatch = severityRe.match(line);
        QString severity = sevMatch.hasMatch() ? sevMatch.captured(1).toUpper() : "INFO";
        severityCounts[severity]++;

        bool isIncident = keywordRe.match(line).hasMatch();
        if (isIncident) m_lastIncidents.append({line, severity});

        if (method == 0 && isIncident) {
            auto *item = new QListWidgetItem(line);
            item->setForeground(QColor("#c62828"));
            resultsList->addItem(item);
        } else if (method == 1) {
            auto *item = new QListWidgetItem(line);
            item->setForeground(colorForSeverity(severity));
            resultsList->addItem(item);
        }
    }

    if (method == 2) {
        resultsList->addItem(QString("Оброблено рядків: %1").arg(lines.size()));
        for (auto it = severityCounts.constBegin(); it != severityCounts.constEnd(); ++it)
            resultsList->addItem(QString("%1: %2").arg(it.key()).arg(it.value()));
    }

    if (resultsList->count() == 0)
        resultsList->addItem("Інцидентів не знайдено");

    int total = 0;
    for (auto v : severityCounts) total += v;
    if (total == 0) total = 1;

    double redPct    = (severityCounts.value("CRITICAL", 0) + severityCounts.value("ERROR", 0)) * 100.0 / total;
    double yellowPct = severityCounts.value("WARNING", 0) * 100.0 / total;
    double greenPct  = severityCounts.value("INFO", 0) * 100.0 / total;

    resultChart->setCenterText("ВСЬОГО РЯДКІВ", QString::number(lines.size()));
    resultChart->setSegments({
        {"critical", redPct, QColor("#e53935")},
        {"warning", yellowPct, QColor("#fbc02d")},
        {"info", greenPct, QColor("#43a047")}
    });

    legendRed->setText(QString("● Критичні: %1%").arg(redPct, 0, 'f', 1));
    legendYellow->setText(QString("● Попередження: %1%").arg(yellowPct, 0, 'f', 1));
    legendGreen->setText(QString("● Інформаційні: %1%").arg(greenPct, 0, 'f', 1));

    m_lastSeverityCounts = severityCounts;
    m_lastMethodName = methodNames.value(method);
    m_lastSourceFile = path;
    m_lastTotalLines = lines.size();
}

void MainWindow::handleSaveJson()
{
    if (m_lastTotalLines == 0) {
        QMessageBox::information(this, "Немає даних", "Спочатку завантажте та проаналізуйте файл логів.");
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, "Зберегти результат аналізу", "analysis_result.json", "JSON files (*.json)");
    if (path.isEmpty()) return;

    QJsonObject root;
    root["source_file"] = m_lastSourceFile;
    root["method"] = m_lastMethodName;
    root["total_lines"] = m_lastTotalLines;
    root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject stats;
    for (auto it = m_lastSeverityCounts.constBegin(); it != m_lastSeverityCounts.constEnd(); ++it)
        stats[it.key()] = it.value();
    root["severity_counts"] = stats;

    QJsonArray incidents;
    for (const auto &inc : m_lastIncidents) {
        QJsonObject o;
        o["line"] = inc.text;
        o["severity"] = inc.severity;
        incidents.append(o);
    }
    root["incidents"] = incidents;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Помилка", "Не вдалося зберегти файл.");
        return;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
    QMessageBox::information(this, "Готово", "Результат збережено.");
}

void MainWindow::handleSaveTxt()
{
    if (m_lastTotalLines == 0) {
        QMessageBox::information(this, "Немає даних", "Спочатку завантажте та проаналізуйте файл логів.");
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, "Зберегти результат аналізу", "analysis_result.txt", "Text files (*.txt)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Помилка", "Не вдалося зберегти файл.");
        return;
    }
    QTextStream out(&file);
    out << "Результат аналізу лог-файлу\n";
    out << "Джерело: " << m_lastSourceFile << "\n";
    out << "Метод: " << m_lastMethodName << "\n";
    out << "Дата: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";
    out << "Всього рядків: " << m_lastTotalLines << "\n\n";

    out << "Статистика за рівнями:\n";
    for (auto it = m_lastSeverityCounts.constBegin(); it != m_lastSeverityCounts.constEnd(); ++it)
        out << "  " << it.key() << ": " << it.value() << "\n";

    out << "\nЗнайдені інциденти:\n";
    for (const auto &inc : m_lastIncidents)
        out << "[" << inc.severity << "] " << inc.text << "\n";

    file.close();
    QMessageBox::information(this, "Готово", "Результат збережено.");
}