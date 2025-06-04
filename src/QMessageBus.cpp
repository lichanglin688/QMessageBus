#include "QMessageBus.h"
#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <QUuid>

QMessageBus* QMessageBus::instance()
{
    static QMessageBus instance;
    return &instance;
}

QMessageObject::QMessageObject(const QString& name, QMessageBus* eventBus, QObject* parent)
    : QObject(parent)
{
	m_name = name.isEmpty() ? QUuid::createUuid().toString(QUuid::Id128) : name;
    m_eventBus = eventBus != nullptr ? eventBus : QMessageBus::instance();
    m_eventBus->registerEventObject(this);
}

QMessageObject::~QMessageObject()
{
    m_eventBus->unregisterEventObject(this);
}

void QMessageObject::send(const QString& targetName, const QVariantHash& message)
{
	m_eventBus->routeMessage(targetName, message);
}

void QMessageBus::registerEventObject(QMessageObject* eventObject)
{
	QWriteLocker locker(&m_rwlock);
    m_eventObjects.insert(eventObject->name(), eventObject);
}

void QMessageBus::unregisterEventObject(QMessageObject* eventObject)
{
	QWriteLocker locker(&m_rwlock);
    m_eventObjects.remove(eventObject->name());
}

void QMessageBus::routeMessage(const QString& targetName, const QVariantHash& message)
{
	QReadLocker locker(&m_rwlock);
    auto iter = m_eventObjects.find(targetName);
    if (iter != m_eventObjects.cend())
    {
        Q_EMIT iter.value()->receive(message);
    }
}

RequestObject::RequestObject(const QString& name, QMessageBus* QMessageBus, QObject* parent)
    : QMessageObject(name, QMessageBus, parent)
{
    connect(this, &QMessageObject::receive, this, &RequestObject::onReceive, Qt::QueuedConnection);
}

int RequestObject::sendRequest(const QString& targetName, const QString& command, const QVariantHash& message)
{
    QVariantHash data;
    data.insert("request", name());
    data.insert("command", command);
    data.insert("message", message);
	int messageId = generateMessageId();
    data.insert("messageId", messageId);
    send(targetName, data);
	return messageId;
}

void RequestObject::onReceive(const QVariantHash& message)
{
    int messageId = message.value("messageId").toInt();
    QVariantHash data = message.value("message").toHash();
    emit receiveReply(messageId, data);
}

int RequestObject::generateMessageId()
{
    return m_messageId++;
}

ResponseObject::ResponseObject(const QString& name, QMessageBus* QMessageBus, QObject* parent)
    : QMessageObject(name, QMessageBus, parent)
{
    connect(this, &QMessageObject::receive, this, &ResponseObject::onReceive, Qt::QueuedConnection);
}

void ResponseObject::reply(const QString& requestName, int messageId, const QVariantHash& message)
{
    QVariantHash data;
    data.insert("messageId", messageId);
    data.insert("message", message);
    send(requestName, data);
}

void ResponseObject::onReceive(const QVariantHash& message)
{
    const QString requestName = message.value("request").toString();
    const QString command = message.value("command").toString();
    const int messageId = message.value("messageId").toInt();
    const QVariantHash data = message.value("message").toHash();
    emit receiveRequest(requestName, command, messageId, data);
}
