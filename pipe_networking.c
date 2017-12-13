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
    } else {
        int upstream_fd = open("upstream_fifo", O_RDONLY);
        char buffer[256];
        read(upstream_fd, buffer, 256);
    }
    return 0;
}


/*=========================
client_handshake
args: int * to_server

Perofrms the client side pipe 3 way handshake.
Sets *to_server to the file descriptor for the upstream pipe.

returns the file descriptor for the downstream pipe.
=========================*/
int client_handshake(int *to_server) {
    return 0;
}
