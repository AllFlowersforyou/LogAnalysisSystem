#ifndef USERSTORE_H
#define USERSTORE_H
#include <QString>
#include <QMap>

namespace UserStore
{
QString filePath();
bool loadUsers(QMap<QString, QString> &users);
bool saveUser(const QString &username, const QString &password);
void ensureDefaultAdmin();
}
#endif