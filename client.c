#include "pipe_networking.h"

int to_server;
int from_server;

static void sighandler(int signo) {
    if (signo == SIGINT) {
        close(to_server);
        close(from_server);
        exit(0);
    }
}

int main() {

    from_server = client_handshake(&to_server);
    
    printf("Please enter input to send to the server:\n");
    signal(SIGINT, sighandler);
    while (1) {
        char send[256];
        fgets(send, 256, stdin);
        send[strlen(send) - 1] = 0;
        write(to_server, send, 256);

        char receive[256];
        read(from_server, receive, 256);
        printf("Received \"%s\"\n", receive);
    }
    // remove private pipe
    close(to_server);
    close(from_server);
}
