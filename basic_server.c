#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  printf("Waiting for connection...\n");
  from_client = server_handshake( &to_client );
  printf("Connection established (waiting for input)\n");
  
  char receive[256];
  read(from_client, receive, 256);
  printf("Received [%s]\n", receive);
  
  char send[256];
  strcpy(send, receive);
  strcat(send, "_MODIFIED");
  write(to_client, send, 256);
  printf("Sent [%s]\n", send);
  // remove upstream pipe
  close(to_client);
  close(from_client);
  char *args[3];
  args[0] = "rm";
  args[1] = "upstream_fifo";
  args[2] = 0;
  execvp(args[0], args);
}
