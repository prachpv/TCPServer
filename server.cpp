#include "server.h"

Server::Server()
{
    if(this->listen(QHostAddress::Any,2323)){
        qDebug()<<"start";
    }else{
        qDebug()<<"error";
    }
    nextBlockSize=0;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    //socket->socketDescriptor()
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,&QTcpSocket::readyRead,this, &Server::slotReadyRead);
    //connect(socket,&QTcpSocket::disconnected,socket, &QTcpSocket::deleteLater);
    connect(socket,&QTcpSocket::disconnected,this, &Server::slotDisconected);
    //qDebug()<<"zha podkluch";
    Sockets.push_back(socket);

    qDebug()<<"client connected"<<socketDescriptor;
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*) sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_10);
    if(in.status()==QDataStream::Ok)
    {
qDebug()<<"read...";
//QString str;
//in>>str;
//qDebug()<<str;
//SendToClient(str);
for( ; ; )
{
    if(nextBlockSize==0)
    {
        qDebug()<<"nextBlockSIze = 0";
        if(socket->bytesAvailable()<2)
        {
             qDebug()<<"Data < 2, break";
            break;
        }
        in>>nextBlockSize;
        qDebug()<<"nextblockSize = "<<nextBlockSize;
    }
    if(socket->bytesAvailable()<nextBlockSize)
    {
        qDebug()<<"Data not full,break";
        break;
    }
    QString str;
    QTime time;

    in>>time>>str;
    nextBlockSize=0;
    SendToClient(str);
    break;
}
    }else{
        qDebug()<<"DataStream error";
    }
}

void Server::slotDisconected()
{
   // removeDescriptor=socket->socketDescriptor();
   // qDebug()<<"removeDescriptor"<<removeDescriptor;
    for(int i=0;i<Sockets.size();i++)
    {
        if(Sockets[i]==socket)
        {
            qDebug()<<i;

            Sockets.remove(i);
            qDebug()<<i;
            //Sockets.clear();
            break;
        }
    }
    socket->deleteLater();
}

void Server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out<<quint16(0)<<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));


    //socket->write(Data);
    for(int i=0;i<Sockets.size();i++)
    {
        Sockets[i]->write(Data);
    }
}
