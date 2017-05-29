#ifndef UNTITLED_H
#define UNTITLED_H
#include <QThread>
#include <QHostAddress>
#include <QUdpSocket>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QNetworkInterface>

struct address{
    QHostAddress LocalAddress =  QHostAddress::QHostAddress("127.0.0.1");
    int LocalPort = 0;
    QHostAddress PublicAddress = QHostAddress::QHostAddress("127.0.0.1");
    int PublicPort = 0;
};

class UdpLibrary : QObject
{


public:
    static UdpLibrary* singleTonInstance;

    UdpLibrary();

    int UdpLibrary::init(QString server, int port);
    void run();
    int enroll(QByteArray token, QByteArray id);
    int connect() ;
    int syncSend() ;
    int asyncSend() ;
    QString set_listen_callback();
    int bindSocket(int port);
    int listen() ;
    static UdpLibrary* getInstance();

    address myAddress;
    address clientAddress;
    address serverAddress;

    QUdpSocket * udpSocket;

};

#endif // UNTITLED_H
