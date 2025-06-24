#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "src/QMessageBus.h"
#include <qcorotask.h>
#include <qcorosignal.h>

class LogService : public QObject
{
	Q_OBJECT
        public:
			LogService(QObject* parent = nullptr) : QObject(parent)
            {
				responseObject = std::make_shared<ResponseObject>(this, "logService", QMessageBus::instance());
				connect(responseObject.get(), &ResponseObject::receiveRequest, this, &LogService::onReceiveRequest);
            }
            ~LogService()
            {
                qDebug() << "LogService destroyed";
			}
private slots:
    void onReceiveRequest(const RequestMessage& requestMessage)
            {
                qDebug() << "LogService received message:" << requestMessage.requestName << requestMessage.command << requestMessage.messageId << requestMessage.message;
                if (requestMessage.command == "debug")
                {
                    responseObject->reply(requestMessage.requestName, requestMessage.messageId, QString("log success"));
                }
            }

private:
	std::shared_ptr<ResponseObject> responseObject;
};

//c++20 协程风格的同步请求函数
QCoro::Task<void> syncRequest(RequestObject* request, const QString& serviceName, const QString& command, const QVariant& message)
{
    int messageId = request->sendRequest(serviceName, command, message);
    auto listener = qCoroSignalListener(request, &RequestObject::receiveReply, std::chrono::seconds(1));
    QCORO_FOREACH(auto messages, listener)
    {
		auto& [responseMessageId, responseMessage] = messages;
        if (responseMessageId == messageId)
        {
			qDebug() << "syncRequest Received response for messageId" << responseMessageId << ":" << responseMessage;
            co_return;
        }
    }
    qDebug() << "Received response Timeout";
    co_return;
}
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QThread thread;
    thread.start();

	LogService* logService = new LogService();
	logService->moveToThread(&thread);

    RequestObject request;

	//不需要响应对象的请求
    request.sendRequest("logService", "info", QString("Hello, World!"));

	//需要响应对象的请求
    syncRequest(&request, "logService", "debug", QString("Hello, World!"));

    syncRequest(&request, "logService", "info", QString("Hello, World!"));

    QTimer::singleShot(3000, &app, [&]() {
		logService->deleteLater();
        thread.quit();
        thread.wait();
        app.quit();
	});
    return app.exec();
}

#include "main.moc"