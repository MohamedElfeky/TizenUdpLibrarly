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

    Q_OBJECT

public:
    static UdpLibrary* singleTonInstance;

    UdpLibrary();

    int UdpLibrary::init(QString server, int port);
    void run();
    int enroll(QByteArray token, QByteArray id);
    int connects() ;
    int syncSend() ;
    int asyncSend() ;
    int bindSocket(int port);
    int listen() ;
    static UdpLibrary* getInstance();

    QUdpSocket * udpSocket;

public slots:
    void set_listen_callback();

private:

    void checkData(QString data);

    address myAddress;
    address clientAddress;
    address serverAddress;
//    QUdpSocket * udpSocket;

    QStringList message;

};

#endif // UNTITLED_H
