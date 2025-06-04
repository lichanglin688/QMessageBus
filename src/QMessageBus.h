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
	explicit QMessageObject(const QString& name = {}, QMessageBus* QMessageBus = nullptr, QObject* parent = nullptr);

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

class RequestObject : public QMessageObject
{
	Q_OBJECT

public:
	explicit RequestObject(const QString& name = {}, QMessageBus* QMessageBus = nullptr, QObject* parent = nullptr);
    int sendRequest(const QString& targetName, const QString& command, const QVariantHash& message);
Q_SIGNALS:
	void receiveReply(int messageId, const QVariantHash& message);

private Q_SLOTS:
	void onReceive(const QVariantHash& message);
private:
	int generateMessageId();

private:
    int m_messageId{};
};

class ResponseObject : public QMessageObject
{
	Q_OBJECT

public:
	explicit ResponseObject(const QString& name = {}, QMessageBus* QMessageBus = nullptr, QObject* parent = nullptr);

	void reply(const QString& requestName, int messageId, const QVariantHash& message);
Q_SIGNALS:
	void receiveRequest(const QString& requestName, const QString& command, int messageId, const QVariantHash& message);

private Q_SLOTS:
	void onReceive(const QVariantHash& message);
};
