#ifndef UNTITLED_H
#define UNTITLED_H
#include <QStringList>
#include <QHostAddress>
#include <QUdpSocket>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QThread>
#include <QtConcurrent/qtconcurrentrun.h>

struct address{
    QHostAddress LocalAddress =  QHostAddress::QHostAddress("127.0.0.1");
    int LocalPort = 0;
    QHostAddress PublicAddress = QHostAddress::QHostAddress("127.0.0.1");
    int PublicPort = 0;
};

enum CONNECT{
    LOCAL_CONNECT,
    PUBLIC_CONNECT,
    RELAY_CONNECT,
    NOT_CONNECTED
};

class UdpLibrary :public QObject
{
    Q_OBJECT
public:
    static UdpLibrary* singleTonInstance;

    UdpLibrary(QObject *parent = 0);
    ~UdpLibrary();

    int UdpLibrary::init(QString server, int port);

    int enroll(QByteArray token, QByteArray id);
    int connects(QByteArray token, QByteArray id);
//    void run();
    int syncSend(QHostAddress address,int port, QString datagram);
//    int asyncSend() ;
    int bindSocket(int port);
//    int listen() ;
    static UdpLibrary* getInstance();

    QUdpSocket * udpSocket;

signals:
    void sendToUser(QStringList message);

public slots:
    void set_listen_callback();
    void connect_other();

private:

    QThread *thread;

    void checkData(QString data);
    void threadStart();

    address myAddress;
    address clientAddress;
    address serverAddress;
//    QUdpSocket * udpSocket;

    QStringList message;
    int connectState = CONNECT::NOT_CONNECTED;
    bool recvf = false;
    bool recvt = false;
    bool start = false;
};

#endif // UNTITLED_H
