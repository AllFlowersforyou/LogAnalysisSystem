#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QLabel>
#include "animatedstackedwidget.h"
#include "animatedbutton.h"
#include "animatedlineedit.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QString loggedInUser() const { return m_loggedInUser; }

private slots:
    void handleLogin();
    void handleRegister();
    void switchToRegister();
    void switchToLogin();
    void animatedMinimize();

private:
    AnimatedStackedWidget *stack;
    QString m_loggedInUser;

    AnimatedLineEdit *loginUserInput, *loginPassInput;
    AnimatedButton *loginButton, *goToRegisterButton;

    AnimatedLineEdit *regUserInput, *regPassInput, *regPassConfirmInput;
    AnimatedButton *registerButton, *goToLoginButton;

    void setupUi();
    QWidget* buildLoginPage();
    QWidget* buildRegisterPage();
    QLabel* buildLogo();
};
#endif