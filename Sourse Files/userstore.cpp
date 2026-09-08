#include "userstore.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>

namespace UserStore
{
QString filePath()
{
    return QCoreApplication::applicationDirPath() + "/users.json";
}

bool loadUsers(QMap<QString, QString> &users)
{
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isArray()) return false;
    for (const QJsonValue &v : doc.array()) {
        QJsonObject obj = v.toObject();
        users.insert(obj.value("username").toString(), obj.value("password").toString());
    }
    return true;
}

bool saveUser(const QString &username, const QString &password)
{
    QMap<QString, QString> users;
    loadUsers(users);
    users.insert(username, password);

    QJsonArray arr;
    for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
        QJsonObject obj;
        obj["username"] = it.key();
        obj["password"] = it.value();
        arr.append(obj);
    }
    QFile file(filePath());
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void ensureDefaultAdmin()
{
    QMap<QString, QString> users;
    loadUsers(users);
    if (!users.contains("admin")) saveUser("admin", "admin");
}
}