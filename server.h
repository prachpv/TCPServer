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
#include <QTextCodec>
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
    bool downloading_in_process=false;
    QByteArray line;
    QFile* file_send;
    QString temp_file_name;
    int k=1;
    QVector<QByteArray> dataVector;
    void SendFile(QString str);
    void SendToClient(QString name,QString str,int mode);
void ReadFile();
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotDisconected();

};

#endif // SERVER_H
