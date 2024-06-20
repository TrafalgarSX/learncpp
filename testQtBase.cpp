#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTimer>
#include <QUrl>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <iostream>


void test_QtDate() {
  // QDate and QDateTime to yyyy-mm-dd hh:mm:ss
  QDate date = QDate::currentDate();
  QDateTime dateTime = QDateTime::currentDateTime();

  std::cout << date.toString("yyyy-MM-dd").toStdString() << std::endl;
  std::cout << dateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString()
            << std::endl;
}

void test_QtBase64() {
  QString str = "Hello, World!";

  QStringList str_list = str.split(" ");

  for (const auto& str : str_list) {
    std::cout << str.toStdString() << std::endl;
  }

  QByteArray byteArray = str.toUtf8();

  QByteArray base64Data = byteArray.toBase64();
  qDebug() << "Encode:" << base64Data;

  std::cout << base64Data.toStdString() << std::endl;

  std::cout << base64Data.toHex().toStdString() << std::endl;
}

void test_QtJson() {
  // JSON 字符串
  QString str = R"({"name":"John", "age":30, "cars":["Ford", "BMW", "Fiat"]})";

  // 解析 JSON 字符串
  QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
  if (!doc.isNull()) {
    if (doc.isObject()) {
      QJsonObject obj = doc.object();
      qDebug() << "Name:" << obj["name"].toString();
      qDebug() << "Age:" << obj["age"].toInt();

      QJsonArray cars = obj["cars"].toArray();
      for (const QJsonValue& value : cars) {
        qDebug() << "Car:" << value.toString();
      }
    } else {
      qDebug() << "Document is not an object";
    }
  } else {
    qDebug() << "Invalid JSON...";
  }
}

int test_QTimer(int argc, char** argv) {
  QCoreApplication a(argc, argv);

  // 创建一个 QTimer 对象
  QTimer timer;

  // 连接 QTimer 的 timeout 信号到我们的 timeout 函数
  // QObject::connect(&timer, &QTimer::timeout, []() {
  //     test_QtDate();
  // });

  QFunctionPointer func = test_QtDate;
  QObject::connect(&timer, &QTimer::timeout, func);

  // 设置定时器每隔 1000 毫秒（1 秒）触发一次
  timer.start(2000);

  // 运行事件循环
  return a.exec();
}

int test_http_request(int argc, char** argv) {
  QCoreApplication a(argc, argv);
#if 1
  QNetworkAccessManager manager;
//   QUrl url("https://www.baidu.com/");
  QUrl url("https://www.google.com/");
  QNetworkRequest request(url);
  QNetworkReply* reply = manager.get(request);

  // 连接请求完成信号
  QObject::connect(reply, &QNetworkReply::finished, [reply]() {
    if (reply->error() == QNetworkReply::NoError) {
      QByteArray data = reply->readAll();
      qDebug() << data;
    } else {
      qDebug() << "Error: " << reply->errorString();
    }

    // 释放资源
    reply->deleteLater();
  });

#endif
  return a.exec();
}

int main(int argc, char** argv) {
  int ret = 0;

  // ret = test_QTimer(argc, argv);
  ret = test_http_request(argc, argv);

  return ret;
}