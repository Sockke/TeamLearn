#include "../include/server.hpp"
#include "../include/service/service.hpp"
#include <signal.h>

void serverQuit(int signo) {
    //Service::instance()->reset();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("input error: ./chatServer ip port");
        return -1;
    }
    const char* ip = argv[1];
    unsigned short port = atoi(argv[2]);

    signal(SIGINT, serverQuit);

    EventLoop loop;
    InetAddress addr(ip, port);
    Server server(&loop, addr, string("Server"));

    server.start();
    loop.loop();
    
    return 0;
}
