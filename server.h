#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#include <QTime>
class Server:public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket* socket;

private:
    QVector<QTcpSocket*> Sockets;
    QByteArray Data;
    quint16 nextBlockSize;
   //int removeDescriptor;
    void SendToClient(QString str);
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotDisconected();
};

#endif // SERVER_H
