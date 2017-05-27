#include "udplibrary.h"
#include <QDebug>

UdpLibrary::UdpLibrary()
{

}

UdpLibrary * UdpLibrary::singleTonInstance= NULL;

int UdpLibrary::enroll(){
    qDebug("library test");
    return 0;
}

int UdpLibrary::connect(){
    return 0;
}
int UdpLibrary::syncSend(){
    return 0;
}
int UdpLibrary::asyncSend(){
    return 0;
}
int UdpLibrary::set_listen_callback(){
    return 0;
}
int UdpLibrary::bindSocket(){
    return 0;
}
int UdpLibrary::listen(){
    return 0;
}
UdpLibrary* UdpLibrary::getInstance(){
   if(UdpLibrary::singleTonInstance == 0){
        UdpLibrary::singleTonInstance = new UdpLibrary();
    }else if(UdpLibrary::singleTonInstance != 0){

    }
    return UdpLibrary::singleTonInstance;
}
