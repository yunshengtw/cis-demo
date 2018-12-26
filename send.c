#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void)
{
    const char host[] = "127.0.0.1";
    const int port = 8887;
    int fd_socket;
    char buf[56];
    struct sockaddr_in server_addr;

    memset(buf, 0, sizeof(buf));
    buf[0] = 10;
    buf[20] = 66;

    fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_socket == 0) {
        perror("socket failed");
        exit(1);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) < 0) {
        perror("inet_pton error");
        exit(1);
    }

    if (connect(fd_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect error");
        exit(1);
    }

    int amount = 56;
    amount = htonl(amount);
    write(fd_socket, (char *)&amount, sizeof(amount));
    write(fd_socket, buf, sizeof(buf));

    while (1);

    return 0;
}
