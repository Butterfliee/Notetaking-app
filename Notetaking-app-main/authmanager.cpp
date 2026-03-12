#include "authmanager.h"
#include <QJsonDocument>
#include <QJsonObject>

AuthManager::AuthManager(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void AuthManager::login(const QString &username, const QString &password) {
    QUrl url("http://127.0.0.1:8000/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    // Propojte to přímo na tenhle konkrétní reply, ne na manager
    connect(reply, &QNetworkReply::finished, this, [=]() {
        onLoginFinished(reply);
    });
}
void AuthManager::onLoginFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        QString token = obj["access_token"].toString();
        int userId = obj["user_id"].toInt();

        emit loginSuccess(token, userId);
    } else {
        emit loginError("Chyba přihlášení: " + reply->errorString());
    }
    reply->deleteLater();
}
void AuthManager::registerUser(const QString &username, const QString &password) {
    QUrl url("http://127.0.0.1:8000/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    // Odeslání dat
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    // Propojení odpovědi se slotem
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit registerSuccess(); // Toto spustí ten qDebug v window.cpp
        } else {
            qDebug() << "Chyba registrace:" << reply->readAll();
        }
        reply->deleteLater();
    });
}
