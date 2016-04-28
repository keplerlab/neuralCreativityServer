
#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <iostream>

enum ClientType
{	sketchApp
};

enum ClientStatus
{
	disconnected, connected
};


class SocketClient : public QObject
{

    Q_OBJECT
public:
	explicit SocketClient(const QUrl &url, ClientType type, QObject *parent = Q_NULLPTR);
	int clientType;
	ClientStatus status; 

	 

Q_SIGNALS:
    void closed();
	void sendMessage(QString message);

public Q_SLOTS:
    void onConnected();
	void onDisconnected();
	void onTextMessageReceivedServer(QString message);

	void sendTextJson(QString message);

public:
    QWebSocket m_webSocket;
    QUrl m_url;
};


#endif // SOCKETCLIENT_H