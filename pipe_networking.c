#include "pipe_networking.h"


/*=========================
server_handshake
args: int * to_client

Perofrms the client side pipe 3 way handshake.
Sets *to_client to the file descriptor to the downstream pipe.

returns the file descriptor for the upstream pipe.
=========================*/
int server_handshake(int *to_client) {
    int upstream_success = mkfifo("upstream_fifo", 0644);
    if (upstream_success < 0) {
        printf("%s\n", strerror(errno));
    }
    int upstream_fd = open("upstream_fifo", O_RDONLY);
    char buffer[256];
    read(upstream_fd, buffer, 256);
    *to_client = open(buffer, O_WRONLY);
    return upstream_fd;
}


/*=========================
client_handshake
args: int * to_server

Perofrms the client side pipe 3 way handshake.
Sets *to_server to the file descriptor for the upstream pipe.

returns the file descriptor for the downstream pipe.
=========================*/
int client_handshake(int *to_server) {
    char s[256];
    sprintf(s, "%d", getpid());
    int success = mkfifo(s, 0644);
    if (success < 0) {
        printf("%s\n", strerror(errno));
    }
    *to_server = open("upstream_fifo", O_WRONLY);
    write(*to_server, s, sizeof(s));
    int downstream_fd = open(s, O_RDONLY);
    return downstream_fd;
}
