#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#include <QTime>
#include <QDir>
#include <QFile>
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
    QFile* file;
    quint64 sizeReceivedData = 0;
    quint64 sizeReceiveFile;
    bool downloading_in_process=false;
    QByteArray line;
    QVector<QByteArray> dataVector;
   //int removeDescriptor;
    void SendToClient(QString name,QString str,int mode);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotDisconected();

};

#endif // SERVER_H
