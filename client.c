#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/uio.h>
#include <unistd.h>

# define SOCKET_FILE "/tmp/unix.sock"

void callMethod(int s) {
  char buf[] = "calldoorapi()";

  if (write(s, buf, strlen(buf)) == -1) {
    fprintf(stderr, "send error\n");
    return;
  }
}

int main(void) {
  int s, cc;
  struct sockaddr_un sa;
  char buf[256];

  /* generate socket */
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* know connection server */
  memset(&sa, 0, sizeof(sa));
  sa.sun_family = AF_UNIX;
  strcpy(sa.sun_path, SOCKET_FILE);

  /* connect server*/
  fprintf(stderr, "Connecting to the server...\n");
  if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    perror("connect");
    exit(1);
  }
  fprintf(stderr, "Connected.\n");

  /* display the message on screen from server*/
  /* fprintf(stderr, "Message from the server:\n");
  while ((cc == read(s, buf, sizeof(buf))) > 0)
    callMethod(s);
  if ((cc == -1)) {
    perror("read");
    exit(1);
  }
  fprintf(stderr, "Connected\n");
  */

  char methods[] = "calldoorapi()";

  if (write(s, methods, strlen(methods)) == -1) {
    fprintf(stderr, "send error\n");
  }
  read(s, methods, strlen(methods));
  printf("methods =%s", methods);

  /* stop connection*/
  if (shutdown(s, SHUT_RDWR) == -1) {
    perror("shutdown");
    exit(1);
  }

  if (close(s) == -1) {
    perror("close");
    exit(1);
  }
  return 0;
}

