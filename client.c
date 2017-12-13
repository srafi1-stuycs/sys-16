#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  printf("Connection established\n");
  printf("Please input text to send to server:\n");
  char send[256];
  fgets(send, 256, stdin);
  send[strlen(send) - 1] = 0;
  write(to_server, send, 256);
  printf("Sent [%s]\n", send);
  char receive[256];
  read(from_server, receive, 256);
  printf("Received [%s]\n", receive);
  // remove private pipe
  close(to_server);
  close(from_server);
}
