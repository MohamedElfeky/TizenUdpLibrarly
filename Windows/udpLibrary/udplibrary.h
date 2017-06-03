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

class connectLibrary :public QObject
{
    Q_OBJECT

public:
    connectLibrary(QObject *parent = 0);

public slots:
    void connect_other();

signals:
    void writeReady(char);

};

class UdpLibrary :public QObject
{
    Q_OBJECT
public:
    static UdpLibrary* singleTonInstance;

    UdpLibrary(QObject *parent = 0);
    ~UdpLibrary();

    connectLibrary *cl;

//    QUdpSocket * udpSocket;
    QThread * connectThread;
    int UdpLibrary::init(QString server, int port);

    int enroll(QByteArray token, QByteArray id);
    int connects(QByteArray token, QByteArray id);
//    void run();

//    int asyncSend() ;
    int bindSocket(int port);
//    int listen() ;
    static UdpLibrary* getInstance();
signals:
    void sendToUser(QStringList message);

public slots:
    void set_listen_callback();
    void checkConnect(char);

private:

    void syncSend(QHostAddress address,int port, QString datagram);

    QThread *thread;
    void connect_other();
    void checkData(QString data);
    void threadStart();

    QUdpSocket * udpSocket;

    QStringList message;
};


#endif // UNTITLED_H
