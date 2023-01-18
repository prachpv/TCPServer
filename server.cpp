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
    qDebug() << "Entered downloading";
            QDir dir;
            QByteArray ba("!@#Rtasd#$%sdfs!!!)()334rrer");
            QByteArray finalBA;
     QString str ="testfile";
            dir.mkpath("C:/Downloads/");
            QFile loadedFile("C:/Downloads/" + str);
            loadedFile.open(QIODevice::WriteOnly | QIODevice::Append);

            while   (socket->bytesAvailable())   {
                line = socket->readAll();
                qDebug() << "str: " << str << "line size: " << line.size();

                dataVector.append(line);
                if (line.contains(ba)) {
                    qDebug() << "Downloaded!";
                    downloading_in_process = false;

                    dataVector.append(line);
                }
            }

            foreach (QByteArray dataBA, dataVector) {
                finalBA.append(dataBA);
            }
            while (finalBA.contains(ba))   {
                int a = line.lastIndexOf(ba);
                qDebug() << "line.lastIndexOf(ba): " << line.lastIndexOf(ba);

                finalBA.remove(a, ba.size()+100);
            }
            int a = line.lastIndexOf(ba);
            finalBA.remove(a, ba.size()+100);
            loadedFile.write(finalBA);
            loadedFile.waitForBytesWritten(30000);
            dataVector.clear();

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

            //Sockets.remove(i);
            Sockets.erase(Sockets.begin()+i);
            qDebug()<<i;
            //Sockets.clear();
            break;
        }
    }
    socket->deleteLater();
}

void Server::SendToClient(QString name,QString str,int mode)
{

    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    //str+="fsfdsf123";

    out<<quint16(0)<<name<<mode<<QTime::currentTime()<<str;

    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));


    //socket->write(Data);
    for(int i=0;i<Sockets.size();i++)
    {

        Sockets[i]->write(Data);
    }
}
