#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QButtonGroup>
#include <QVector>
#include <QMap>
#include "animatedstackedwidget.h"
#include "circularprogress.h"

class Sidebar;

struct ParsedIncident { QString text; QString severity; };

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &currentUser, QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void loggedOut();

private slots:
    void handleUploadFile();
    void handleChangePassword();
    void handleSaveJson();
    void handleSaveTxt();
    void animatedMinimize();
    void onNavSelected(int index);

private:
    Sidebar *sidebar;
    AnimatedStackedWidget *contentStack;
    CircularProgress *resultChart;
    QListWidget *resultsList;
    QButtonGroup *methodGroup;
    QLabel *legendRed, *legendYellow, *legendGreen;
    QString m_currentUser;
    bool m_navLocked = false;

    QVector<ParsedIncident> m_lastIncidents;
    QMap<QString, int> m_lastSeverityCounts;
    QString m_lastMethodName;
    QString m_lastSourceFile;
    int m_lastTotalLines = 0;

    void setupUi();
    QWidget* buildHomeTab();
    QWidget* buildMethodsTab();
    QWidget* buildAccountTab();
    QLabel* buildWatermark();
    QColor colorForSeverity(const QString &sev) const;
};
#endif