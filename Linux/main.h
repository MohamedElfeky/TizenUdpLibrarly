#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <string>
class udp_simple_socket
{
public:
                        udp_simple_socket(const std::string& addr, int port);
                        ~udp_simple_socket();

    int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    bool                listen() const;
    int                 sendSync(const char *msg, size_t size);
    // int                 sendAsync(const char *msg, size_t size);

    void                recv() const;//for문 돌면서 listen
    void                send(string message) const;//send 하기

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
    static void *       startRecv(void * This);
    static void *       startSend(void * This);
};