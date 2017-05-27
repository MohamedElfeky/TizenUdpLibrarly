#ifndef UNTITLED_H
#define UNTITLED_H


class UdpLibrary
{


public:
    static UdpLibrary* singleTonInstance;

    UdpLibrary();
    int enroll() ;
    int connect() ;
    int syncSend() ;
    int asyncSend() ;
    int set_listen_callback();
    int bindSocket() ;
    int listen() ;
    UdpLibrary* getInstance();

};

#endif // UNTITLED_H
