#include "udplibrary.h"
#include <QDebug>

UdpLibrary::UdpLibrary()
{
    qDebug("udp start");
    udpSocket = new QUdpSocket(this);
    if(!udpSocket){
        qDebug() << "socket error";
    }
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

     for(int nIter=0; nIter<list.count(); nIter++)
      {
          if(!list[nIter].isLoopback())
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){
                  myAddress.LocalAddress = list[nIter].toString();
                  break;
              }

      }
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(set_listen_callback()));
}

int UdpLibrary::init(QString server, int port)
{
    qDebug("start init");

    QHostAddress serverIP = QHostAddress::QHostAddress(server);

    qDebug(server.toUtf8());
    serverAddress.LocalAddress = serverIP;
    qDebug(serverAddress.LocalAddress.toString().toUtf8());
    serverAddress.LocalPort = port;
    qDebug("end init");
    return 0;
}

UdpLibrary * UdpLibrary::singleTonInstance= NULL;

int UdpLibrary::enroll(QByteArray token, QByteArray id){

    qDebug("library test");
    udpSocket -> connectToHost(serverAddress.LocalAddress, serverAddress.LocalPort, QIODevice::ReadWrite);
    // data from external function
    QByteArray datagram = "";
    datagram = "e|" + token + "|" + id + "|";
    datagram += myAddress.LocalAddress.toString().toUtf8() + "|" + QByteArray::number(myAddress.LocalPort) + "|";
    udpSocket->write(datagram);
    udpSocket->disconnectFromHost();
    this->bindSocket(myAddress.LocalPort);
    return 0;
}

int UdpLibrary::connects(){
    return 0;
}

int UdpLibrary::syncSend(){
    return 0;
}

int UdpLibrary::asyncSend(){
    return 0;
}

void UdpLibrary::set_listen_callback(){
    qDebug("recv success");
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();

        checkData(datagram.data());
        //processTheDatagram(datagram);

        //return datagram.data();
    }
}

void UdpLibrary::checkData(QString data){
    qDebug(data.toUtf8());
    message = data.split("|");      // for example, message is "hello|world|"
    qDebug() << message.count();    // count = 3, message[0] = "hello", message[1] = "world" ,message[2] = ""

    if(!message.isEmpty()){
        if(message[0] == "c" && message.count() == 8){
            myAddress.PublicAddress = message[1]; myAddress.PublicPort = (message[2]).toInt();
            clientAddress.PublicAddress = message[3]; clientAddress.PublicPort = (message[4]).toInt();
            clientAddress.LocalAddress = message[5]; clientAddress.LocalPort = (message[6]).toInt();
        }
        qDebug() << myAddress.PublicAddress;
        qDebug() << QString::number(myAddress.PublicPort);
    }
}

int UdpLibrary::bindSocket(int port){
    myAddress.LocalPort = port;
    udpSocket->bind(myAddress.LocalPort,QUdpSocket::ShareAddress);
    //udpSocket->bind(QHostAddress::Any,3456);
    return 0;
}

UdpLibrary* UdpLibrary::getInstance(){
   if(UdpLibrary::singleTonInstance == 0){
        UdpLibrary::singleTonInstance = new UdpLibrary();
    }else if(UdpLibrary::singleTonInstance != 0){

    }
    return UdpLibrary::singleTonInstance;
}
