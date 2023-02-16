#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

using namespace std;
struct player_t {
    int id;
    int fd;
};
typedef struct player_t player;
class Server {
   public:
    int init(const char *port) {
        int status;
        int socket_fd;
        struct addrinfo host_info;
        struct addrinfo *host_info_list;
        const char *hostname = NULL;
        memset(&host_info, 0, sizeof(host_info));
        host_info.ai_family = AF_UNSPEC;
        host_info.ai_socktype = SOCK_STREAM;
        host_info.ai_flags = AI_PASSIVE;

        status = getaddrinfo(hostname, port, &host_info, &host_info_list);
        if (status != 0) {
            cerr << "Error: cannot get address info for host" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        socket_fd = socket(host_info_list->ai_family,
                           host_info_list->ai_socktype,
                           host_info_list->ai_protocol);
        if (socket_fd == -1) {
            cerr << "Error: cannot create socket" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        int yes = 1;
        status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
        if (status == -1) {
            cerr << "Error: cannot bind socket" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        status = listen(socket_fd, 100);
        if (status == -1) {
            cerr << "Error: cannot listen on socket" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        cout << "Waiting for connection on port " << port << endl;

        freeaddrinfo(host_info_list);

        return socket_fd;
    }
};
class Client {
   public:
    int init(const char *hostname, const char *port) {
        int status;
        int socket_fd;
        struct addrinfo host_info;
        struct addrinfo *host_info_list;

        memset(&host_info, 0, sizeof(host_info));
        host_info.ai_family = AF_UNSPEC;
        host_info.ai_socktype = SOCK_STREAM;

        status = getaddrinfo(hostname, port, &host_info, &host_info_list);
        if (status != 0) {
            cerr << "Error: cannot get address info for host" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        socket_fd = socket(host_info_list->ai_family,
                           host_info_list->ai_socktype,
                           host_info_list->ai_protocol);
        if (socket_fd == -1) {
            cerr << "Error: cannot create socket" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        cout << "Connecting to " << hostname << " on port " << port << "..." << endl;

        status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
        if (status == -1) {
            cerr << "Error: cannot connect to socket" << endl;
            cerr << "  (" << hostname << "," << port << ")" << endl;
            return -1;
        }  // if

        freeaddrinfo(host_info_list);

        return socket_fd;
    }
};