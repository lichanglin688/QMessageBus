#pragma once
#include <QObject>
#include <QString>
#include <QHash>
#include <QVariant>
#include <QMutex>
#include <QReadWriteLock>

class QMessageBus;

class QMessageObject : public QObject
{
	Q_OBJECT
		friend class QMessageBus;

public:
	explicit QMessageObject(QObject* parent = nullptr, const QString& name = {}, QMessageBus* QMessageBus = nullptr);

	~QMessageObject();

	QString name() const { return m_name; }

	void send(const QString& targetName, const QVariantHash& message);

Q_SIGNALS:
	void receive(const QVariantHash& message);
private:
	QString m_name;
	QMessageBus* m_eventBus;
};

class QMessageBus : public QObject
{
	Q_OBJECT

public:
	static QMessageBus* instance();

	void registerEventObject(QMessageObject* eventObject);
	void unregisterEventObject(QMessageObject* eventObject);

public Q_SLOTS:
	void routeMessage(const QString& targetName, const QVariantHash& message);

private:
	QHash<QString, QMessageObject*> m_eventObjects;
	QReadWriteLock m_rwlock;
};

class RespnseMessage
{
public:
	RespnseMessage(int messageId, const QVariant& message)
		: messageId(messageId), message(message)
	{
	}
	template<typename T>
	T getResponse() const
	{
		return message.value<T>();
	}
	QVariant getResponse() const
	{
		return message;
	}
	int getMessageId() const
	{
		return messageId;
	}
private:
	int messageId{};
	QVariant message;
};

class RequestObject : public QMessageObject
{
	Q_OBJECT

public:
	explicit RequestObject(QObject* parent = nullptr, const QString& name = {}, QMessageBus* QMessageBus = nullptr);
	template<typename T>
	int sendRequest(const QString& targetName, const QString& command, const T& message)
	{
		return sendRequest(targetName, command, QVariant::fromValue(message));
	}
	int sendRequest(const QString& targetName, const QString& command, const char* message)
	{
		return sendRequest(targetName, command, QVariant::fromValue(QString(message)));
	}
	int sendRequest(const QString& targetName, const QString& command, const QVariant& message);
Q_SIGNALS:
	void receiveReply(const RespnseMessage& message);

private Q_SLOTS:
	void onReceive(const QVariantHash& message);
private:
	int generateMessageId();

private:
	int m_messageId{};
};

class RequestMessage
{
public:
	RequestMessage(const QString& requestName = {}, const QString& command = {}, int messageId = 0, const QVariant& message = {})
		: requestName(requestName), command(command), messageId(messageId), message(message)
	{
	}
	template<typename T>
	T getMessage() const
	{
		return message.value<T>();
	}
	QVariant getMessage() const
	{
		return message;
	}
	int getMessageId() const
	{
		return messageId;
	}
	QString getRequestName() const
	{
		return requestName;
	}
	QString getCommand() const
	{
		return command;
	}
private:
	QString requestName;
	QString command;
	int messageId{};
	QVariant message;
};
class ResponseObject : public QMessageObject
{
	Q_OBJECT

public:
	explicit ResponseObject(QObject* parent = nullptr, const QString& name = {}, QMessageBus* QMessageBus = nullptr);

	void reply(const QString& requestName, int messageId, const QVariant& message);
Q_SIGNALS:
	void receiveRequest(const RequestMessage& requestMessage);

private Q_SLOTS:
	void onReceive(const QVariantHash& message);
};
