#include "ioscheduler.h"
#include "hook.h"  

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>

// 服务器地址和端口
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 8080

// 要并发发起的请求数量
#define NUM_REQUESTS 10

void error(const char *msg) {
    perror(msg);
    exit(1);
}
static int sock = -1;
// 执行一次 HTTP GET 请求
void http_client_task(int i) {  // 加个 id 便于调试
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "socket failed\n";
        return;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "connect failed\n";
        close(sock);
        return;
    }

    const char *request = "GET / HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Connection: close\r\n\r\n";

    send(sock, request, strlen(request), 0);

    char buffer[4096];
    std::string response;
    ssize_t n;

    while ((n = recv(sock, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[n] = '\0';
        response += buffer;
    }

    std::cout << "Client " << i << " received: " << response << " bytes\n";

    close(sock); // 关闭连接
}

int main() {
    std::cout << "Starting " << NUM_REQUESTS << " HTTP clients...\n";

    for (int i = 0; i < NUM_REQUESTS; ++i) {
        http_client_task(i); // 每次新建连接
    }

    return 0;
}