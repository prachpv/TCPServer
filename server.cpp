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
    k++;
    int mode;
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


        in>>name>>mode>>time;
        if(true)
        {
            in>>str;
        }
        qDebug()<<"mode-> "<<QString::number(mode)<<"   "<<name<<" send -> "<<str;
        nextBlockSize=0;
        //SendToClient(name,str,mode);
        break;
    }
        }else{
            qDebug()<<"DataStream error";
        }


if(true)
{
   ReadFile();
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
//void Server::SendFile(QString str)
//{
//    QString filePathName = file_path_temp;
//        QStringList filePath = filePathName.split("/");

//        QFile file(file_path_temp);
//        QByteArray q;
//        int i = 0;
//        QByteArray block;
//        QByteArray arrBlock;
//        QByteArray junk("!@#Rtasd#$%sdfs!!!)()334rrer");
//        QList<QByteArray> Blocks;
////_mode=1;
//        if (file.open(QIODevice::ReadOnly)) {
//            QFileInfo fileInfo(file);
//            QString fileName(filePath.last());

//            QDataStream out(&arrBlock, QIODevice::WriteOnly);
//            out.setVersion(QDataStream::Qt_5_7);
//            //

////            out << quint16(0) << QTime::currentTime()
////                <<username<<fileName << fileInfo.size();

////            out.device()->seek(0);
////            out << quint16(arrBlock.size() - sizeof(quint16));
//            //


//            while (!file.atEnd())
//            {
//                q = file.readAll();
//                file.close();
//            }
//    //        Blocks = q.split('\n');
//            int pos = 0, arrsize = q.size(), sizeInArray = 512;
//            while(pos<arrsize){
//                QByteArray arr = q.mid(pos, sizeInArray);
//                Blocks << arr;
//                pos+=arr.size();
//            }

//            foreach (block, Blocks) {

//                socket->write(block);
//                socket->waitForBytesWritten();
//                if (block == Blocks.last()) {
//                    qDebug() << "sended!";
//                    socket->write(junk);
//                    socket->waitForBytesWritten();
//                    block.clear();
//                }
//                q.clear();
//                i++;
//                //ui->prBr_fileSending->setValue(i/Blocks.size()*100);
//            }
//        }
//}
void Server::ReadFile()
{
    //    socket = (QTcpSocket*) sender();
    //    QDataStream in(socket);
        qDebug() << "Entered downloading";
                QDir dir;
                QByteArray ba("!@#Rtasd#$%sdfs!!!)()334rrer");
                QByteArray finalBA;
         QString str =QString::number(k);
         str="testfile.txt";
                dir.mkpath("C:/Downloads/");
                QFile loadedFile("C:/Downloads/" + str);
                if(loadedFile.open(QIODevice::WriteOnly | QIODevice::Append)){


                while   (socket->bytesAvailable())   {

                    line = socket->readAll();
                    QString temp=QString(line);

                    qDebug() << "str: " << str << "line size: " << line.size()<<"\t text line-> "<<line;

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

                QString strtemp = QTextCodec::codecForMib(1015)->toUnicode(finalBA);
                qDebug()<<finalBA.size()<<"<_>"<<strtemp;
                //
                //
                loadedFile.write(finalBA);

                loadedFile.waitForBytesWritten(30000);
                //SendFile("C:/DownloadsServer/"+str);

                dataVector.clear();
                }
}



