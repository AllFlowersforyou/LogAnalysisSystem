#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    while (true) {
        LoginDialog login;
        if (login.exec() != QDialog::Accepted)
            return 0;

        bool loggedOut = false;
        MainWindow w(login.loggedInUser());
        QObject::connect(&w, &MainWindow::loggedOut, &a, [&loggedOut, &a]() {
            loggedOut = true;
            a.quit();
        });
        w.show();
        a.exec();

        if (!loggedOut)
            return 0;
    }
}