#include "pipe_networking.h"


/*=========================
server_handshake
args: int * to_client

Perofrms the client side pipe 3 way handshake.
Sets *to_client to the file descriptor to the downstream pipe.

returns the file descriptor for the upstream pipe.
=========================*/
int server_handshake(int *to_client) {
	printf("STARTING HANDSHAKE\n");
	// setup
    int upstream_success = mkfifo("upstream_fifo", 0644);
    if (upstream_success < 0) {
        printf("%s\n", strerror(errno));
    }
    
	printf("Waiting for client to connect...\n");
    int upstream_fd = open("upstream_fifo", O_RDONLY);
    char client_pid[256];
    read(upstream_fd, client_pid, 256);
    printf("Received initial message from client\n");
    if (!fork()) {
        char *args[3];
        args[0] = "rm";
        args[1] = "upstream_fifo";
        args[2] = 0;
        execvp(args[0], args);
    }

    *to_client = open(client_pid, O_WRONLY);
    char send[256] = "hello";
    write(*to_client, send, 256);
    
    char client_response[256];
    read(upstream_fd, client_response, 256);
    printf("Received \"%s\" from client\n", client_response);
    printf("HANDSHAKE DONE\n");
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
	printf("STARTING HANDSHAKE\n");
	// setup
    char my_pid[256];
    sprintf(my_pid, "%d", getpid());
    int success = mkfifo(my_pid, 0644);
    if (success < 0) {
        printf("%s\n", strerror(errno));
    }
    
	printf("Attempting to write to server...\n");
    *to_server = open("upstream_fifo", O_WRONLY);
    write(*to_server, my_pid, 256);
    printf("PID sent\n");

    int downstream_fd = open(my_pid, O_RDONLY);
    char server_response[256];
    read(downstream_fd, server_response, 256);
    printf("Received \"%s\" from server\n", server_response);
    if (!fork()) {
        char *args[3];
        args[0] = "rm";
        args[1] = my_pid;
        args[2] = 0;
        execvp(args[0], args);
    }

	char client_response[256] = "guten tag";
	write(*to_server, client_response, 256);
	printf("HANDSHAKE DONE\n");
    return downstream_fd;
}
