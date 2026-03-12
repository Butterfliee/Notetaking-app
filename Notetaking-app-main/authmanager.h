#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AuthManager : public QObject {
    Q_OBJECT
public:
    explicit AuthManager(QObject *parent = nullptr);
    void login(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &password);
signals:
    void registerSuccess();

signals:
    void loginSuccess(QString token, int userId);
    void loginError(QString message);

private slots:
    void onLoginFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif
