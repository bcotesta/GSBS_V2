#include "TwilioService.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <iostream>

TwilioService& TwilioService::getInstance() {
    static TwilioService instance;
    return instance;
}

TwilioService::TwilioService() {
    // No global initialization needed for Qt Network
}

TwilioService::~TwilioService() {
    // No global cleanup needed for Qt Network
}

void TwilioService::configure(const std::string& accountSid, 
                              const std::string& authToken, 
                              const std::string& fromNumber) {
    accountSid_ = accountSid;
    authToken_ = authToken;
    fromNumber_ = fromNumber;
    
    std::cout << "[TWILIO] Configured successfully" << std::endl;
    std::cout << "[TWILIO] From Number: " << fromNumber_ << std::endl;
}

std::string TwilioService::urlEncode(const std::string& value) {
    QUrl url;
    QString encoded = QString::fromStdString(value);
    return QUrl::toPercentEncoding(encoded).toStdString();
}

bool TwilioService::sendSMS(const std::string& toNumber, const std::string& message) {
    if (accountSid_.empty() || authToken_.empty() || fromNumber_.empty()) {
        std::cerr << "[TWILIO] Error: Twilio not configured. Call configure() first." << std::endl;
        return false;
    }
    
    // Create network manager
    QNetworkAccessManager manager;
    
    // Build Twilio API URL
    QString urlString = QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json")
                        .arg(QString::fromStdString(accountSid_))
                        ;
    QUrl url(urlString);
    
    // Create request
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    // Set basic authentication
    QString credentials = QString("%1:%2")
                          .arg(QString::fromStdString(accountSid_))
                          .arg(QString::fromStdString(authToken_))
                          ;
    QByteArray authData = credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + authData);
    
    // Build POST data
    QUrlQuery postData;
    postData.addQueryItem("To", QString::fromStdString(toNumber));
    postData.addQueryItem("From", QString::fromStdString(fromNumber_));
    postData.addQueryItem("Body", QString::fromStdString(message));
    
    QByteArray postDataEncoded = postData.toString(QUrl::FullyEncoded).toUtf8();
    
    // Send request
    QNetworkReply* reply = manager.post(request, postDataEncoded);
    
    // Wait for response (synchronous approach for simplicity)
    QEventLoop loop;
    QTimer timeout;
    timeout.setSingleShot(true);
    
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);
    
    timeout.start(10000); // 10 second timeout
    loop.exec();
    
    bool success = false;
    
    if (reply->isFinished()) {
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (reply->error() == QNetworkReply::NoError && httpStatus >= 200 && httpStatus < 300) {
            std::cout << "[TWILIO] SMS sent successfully to " << toNumber << std::endl;
            success = true;
        } else {
            std::cerr << "[TWILIO] Error: HTTP " << httpStatus << std::endl;
            std::cerr << "[TWILIO] Error: " << reply->errorString().toStdString() << std::endl;
            
            QByteArray response = reply->readAll();
            if (!response.isEmpty()) {
                std::cerr << "[TWILIO] Response: " << response.toStdString() << std::endl;
            }
            success = false;
        }
    } else {
        std::cerr << "[TWILIO] Error: Request timed out" << std::endl;
        reply->abort();
        success = false;
    }
    
    reply->deleteLater();
    
    return success;
}