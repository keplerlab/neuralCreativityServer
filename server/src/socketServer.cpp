
#include "socketServer.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>



#include "json/json.h"
#include <thread>
#include <chrono>
#include <iostream>

#include "helper.h"

using namespace std;

extern volatile bool sketchRecivedState ;
extern std::string saveFolderName;

QT_USE_NAMESPACE

SocketServer::SocketServer(quint16 port, QObject *parent) :
QObject(parent),
m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Socket Server"),
                                        QWebSocketServer::NonSecureMode, this)),
m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Socketserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &SocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &SocketServer::closed);
        
        
        connect(this, &SocketServer::sendMessage,
                this, &SocketServer::sendTextJson);
        
        
    }
}

SocketServer::~SocketServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void SocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    
    connect(pSocket, &QWebSocket::textMessageReceived, this, &SocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &SocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SocketServer::socketDisconnected);
    
    m_clients << pSocket;
}

void SocketServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        //qDebug() << "Message " << message;
        
        
        Json::Value root;   // will contains the root value after parsing.
        Json::Reader reader;
        std::string test = message.toStdString();
        
        bool parsingSuccessful = reader.parse(test, root, false);
        if (!parsingSuccessful)
        {
            // report to the user the failure and their locations in the document.
            std::cerr << "JSON Parsing Errors: ";
            std::cerr << reader.getFormatedErrorMessages() << endl;
            return;
        }

        
     //   Json::Value imageEvent;
     //   imageEvent["cameraEvent"] = "image";
     //   imageEvent["Value"] = encoded.c_str();
        
        const Json::Value eventType = root["sketchEvent"];
        
        if (!eventType.isNull())
        {
            
            //std::string eventString = eventType.asString()
            if (eventType.asString() == "image")
            {
                //	cout << "magnetState is 0" << endl;
                qDebug() << "\n\nImage Recived"  << endl ;
                
                sketchRecivedState = true;
                
                const Json::Value imageStr = root["Value"];
                
                const Json::Value xyData = root["xydata"];
                
                
//                std::string imageString = imageEvent.toStyledString();
//                QString imageStr = QString::fromUtf8(imageString.c_str());
//                emit server->sendMessage(imageStr);
                
				//cout << xyData;
                std::string wapis = imageStr.asString() ;
                std::string decoded = base64_decode(wapis);
                
				ofstream xyOut("xytest.txt");
                ofstream fout("test.png", ios::binary);//for testing purpose, to see if the string is a right copy
                
                fout << decoded;
				xyOut << xyData;
				xyOut.close();
                
                qDebug() << "\n\n#Base64 Image Recieved suceess# \n\n" << endl ;
                
                //qDebug() << "Encoded string is" << imageStr;
        
                
            }
            
            
            
        }
        

        
        // pClient->sendTextMessage(message);
    }
    //    m_pWebSocketServer->close();
}

void SocketServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        qDebug() << "Message " << message;
        //pClient->sendBinaryMessage(message);
    }
}

void SocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}


void SocketServer::sendTextJson(QString str)
{
    //QString str = QString::fromUtf8(msg.c_str());
    if (!this->m_clients.empty())
    {
        // QList<QWebSocket *>
        for (QList<QWebSocket *>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
        {
            //	std::cout << ' ' << *it;
            
            QWebSocket *pClient = *it;
            pClient->sendTextMessage(str);
            
        }
        
    }
    else
    {
        qDebug() << "\n\nNo clients connected cannot send msg: " << str << endl;
    }
}