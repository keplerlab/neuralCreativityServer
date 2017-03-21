
#include "socketClient.h"
#include <QtCore/QDebug>

#include "json/json.h"
#include <thread>
#include <chrono>

using namespace std; 

enum
{
	journeyStopped = 0,
	journeyStarted = 1
};





QT_USE_NAMESPACE

SocketClient::SocketClient(const QUrl &url,ClientType type,QObject *parent) :
    QObject(parent),
	m_url(url), clientType(type), status(ClientStatus::disconnected)
{
//	qRegisterMetaType<QAbstractSocket::SocketState>();
	connect(&m_webSocket, &QWebSocket::connected, this, &SocketClient::onConnected);
	connect(&m_webSocket, &QWebSocket::disconnected, this, &SocketClient::onDisconnected);

	qDebug() << "Trying to connect";

	m_webSocket.open(QUrl(url));

//	this->onStart();
}


void SocketClient::onConnected()
{
    qDebug() << "WebSocket connected";
	status = ClientStatus::connected;
    if (clientType == ClientType::sketchApp)
	{
		connect(&m_webSocket, &QWebSocket::textMessageReceived,
			this, &SocketClient::onTextMessageReceivedServer);
		connect(this, &SocketClient::sendMessage,
			this, &SocketClient::sendTextJson);
	}

    //m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}


void SocketClient::onDisconnected()
{
	status = ClientStatus::disconnected;
		qDebug() << "WebSocket server disconnected ... Trying to reconnect";

//	m_webSocket.connect;
	//while (1)
	//{
	// Wait for 2 sec and try to reconnect 
	std::this_thread::sleep_for(std::chrono::seconds(2));
	m_webSocket.open(this->m_url);

	//}

	//emit &SocketClient::closed;

	// Try reconnecting if websocket disconnected 

//	connect(&m_webSocket, &QWebSocket::textMessageReceived,
//		this, &SocketClient::onTextMessageReceived);
	//m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}


void SocketClient::onTextMessageReceivedServer(QString message)
{
    //qDebug() << "Message received From server :" << message;

	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	std::string test = message.toStdString();

	bool parsingSuccessful = reader.parse(test, root, false);
	if (!parsingSuccessful)
	{
		// report to the user the failure and their locations in the document.
		std::cerr << "JSON Parsing Errors: ";
		std::cerr << reader.getFormattedErrorMessages() << endl;
		return;
	}
    else
    {
        cout << "Result: \n" << root.toStyledString() << endl ; 
    }
    
    
	
}

void SocketClient::sendTextJson(QString str)
{
	//QString str = QString::fromUtf8(msg.c_str());
	m_webSocket.sendTextMessage(str);

}
