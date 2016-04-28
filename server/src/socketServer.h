
#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)


class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(quint16 port, QObject *parent = Q_NULLPTR);
    ~SocketServer();
    
Q_SIGNALS:
    void closed();
    
    void sendMessage(QString message);
    
    
    private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void sendTextJson(QString str);
    
private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif //SOCKETSERVER_H