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


    QString str="";
    QTime time;
QString name;
int mode;

    in>>name>>mode>>time;
    if(mode==0)
    {

        in>>str;
        qDebug()<<"mode-> "<<QString::number(mode)<<"   "<<name<<" send -> "<<str;
    }
    QString filename;
    //quint64 size;
    if(mode==1)
    {
        in>>filename>>sizeReceiveFile;
        qDebug() << "Size Recieve:  " << sizeReceiveFile << "FileName: " << filename;
        receiveFile(filename);
        break;
    }


    //SendToClient(name,str,mode);
   nextBlockSize=0;
}
    }else{
        qDebug()<<"DataStream error";
    }
}

void Server::receiveFile(QString fileName) {
    QString savePath = "C://";
    QDir dir;
    dir.mkpath(savePath);
    file = new QFile(savePath + fileName);
    file->open(QFile::WriteOnly);
    sizeReceivedData = 0;
    receiveFile();
}
void Server::receiveFile() {
    QDataStream in(socket);
    char block[1024];
    while(!in.atEnd()){
        qint64 toFile = in.readRawData(block, sizeof(block));
        sizeReceivedData += toFile;
        qDebug() << "sizeReceivedData: " << sizeReceivedData;
        file->write(block, toFile);
        if(sizeReceivedData == sizeReceiveFile){
            file->close();
            file = NULL;
            qDebug() << "sizeReceivedData END: " << sizeReceivedData;
            sizeReceiveFile = 0;
            sizeReceivedData = 0;
        }
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
