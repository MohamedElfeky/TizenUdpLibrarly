#include "udplibrary.h"
#include <QDebug>
#include <QThread>

UdpLibrary::UdpLibrary(QObject *parent)
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

     //thread = new QThread();
     connect(udpSocket,SIGNAL(readyRead()),this,SLOT(set_listen_callback()));

}

UdpLibrary::~UdpLibrary(){
    delete this->udpSocket;
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
void UdpLibrary::connect_other(){
    qDebug() << "connect start";
    qDebug() << "currentState : " << connectState;
    int i;
        for(i=0;i<5;i++){										// 로컬 연결 시도
            if(!recvf){
                syncSend(clientAddress.LocalAddress,clientAddress.LocalPort,"l|f");			//연결안되면 l|f
            }
            else{
                syncSend(clientAddress.LocalAddress,clientAddress.LocalPort,"l|t");			//연결안되면 l|f
                }
            QThread::usleep(10000);
        }
        QThread::usleep(100000);
        if(recvt){
            connectState = CONNECT::LOCAL_CONNECT;				//현재 연결을 LOCAL 로 설정
        }
        else{
            connectState = CONNECT::NOT_CONNECTED;					//글로벌 연결 시도
            recvf = false;
            for(i=0;i<5;i++){										// 로컬 연결 시도
                if(!recvf){
                    syncSend(clientAddress.PublicAddress,clientAddress.PublicPort,"r|f");			//연결안되면 l|f
                }
                else{
                    syncSend(clientAddress.PublicAddress,clientAddress.PublicPort,"r|t");			//연결안되면 l|f
                }
                QThread::usleep(10000);
            }
            QThread::usleep(100000);
            if(recvt){
                connectState = CONNECT::PUBLIC_CONNECT;
            }
            else{
                connectState = CONNECT::NOT_CONNECTED;					//글로벌 연결 시도
                recvf = false;
                for(i=0;i<5;i++){
                    if(!recvf){
                        syncSend(serverAddress.LocalAddress,serverAddress.LocalPort,"r|f");			//연결안되면 l|f
                    }
                    else{
                        syncSend(serverAddress.LocalAddress,serverAddress.LocalPort,"r|t");			//연결안되면 l|f
                    }
                    QThread::usleep(10000);
                }
                QThread::usleep(100000);
            }
            if(recvt){
                connectState = CONNECT::RELAY_CONNECT;
            }
        }
        qDebug() << "currentState : " << connectState;
}


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

int UdpLibrary::connects(QByteArray token, QByteArray id){
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


int UdpLibrary::syncSend(QHostAddress address,int port, QString datagram){
    udpSocket -> connectToHost(address, port, QIODevice::ReadWrite);
    udpSocket->write(datagram.toUtf8());
    udpSocket->disconnectFromHost();
    this->bindSocket(myAddress.LocalPort);
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

void UdpLibrary::threadStart(){
    /*if(thread->isRunning()){
        thread->quit();
        qDebug() << "thread exit()";
        thread->wait();
        qDebug() << "thread exit()";
        thread->terminate();
        qDebug() << "thread exit()";
        thread->destroyed();
        qDebug() << "thread exit()";
        delete thread;
        qDebug() << "thread exit()";
        thread = new QThread();
        qDebug() << "thread exit()";
    }*/
    thread = new QThread();
    connect(thread, &QThread::finished, this, &QObject::deleteLater);
    connect(thread, SIGNAL(started()), this, SLOT(connect_other()));
    moveToThread(thread);
    udpSocket->bind(myAddress.LocalPort);
    qDebug() << "tt";
    thread->start();
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
            threadStart();
            //QFuture<void> future = QtConcurrent::run( this , &UdpLibrary::connect_other );
        }
        else if(message[0] == "m"){
            message.removeFirst();
            emit sendToUser(message);
        }
        else if(message[0] == "l" || message[0] == "g" || message[0] == "t" ){
            if (message[1][0] == 'f'){
                recvf = true;
            }
            else if (message[1][0] == 'f')
                recvt = true;
        }
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
