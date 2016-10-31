#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <unistd.h>

# define SOCKET_FILE "socket"
enum {
    NQUEUE_SIZE = 5,
};

char *message = "hello!¥Good Bye!!\n";

int main(void) {
    int s, ws, cc;
    struct sockaddr_un sa, ca;
    const int soval = 1;
    socklen_t ca_len;

    /* generate socket */
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
    }

    /* reuse address */
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1 ) {
      perror("setsockopt");
      exit(1);
    }

    /* name socker */
    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, SOCKET_FILE);
    if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
      perror("bind");
      exit(1);
    }

    /* prepare queue */
    if (listen(s, NQUEUE_SIZE)) {
      perror("listen");
      exit(1);
    }

    fprintf(stderr, "Ready. \n");

    for (;;) {
      fprintf(stderr, "Wanting for a connection...\n");
      ca_len = sizeof(ca);
      if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
        perror("accept");
        exit(1);
      }

      fprintf(stderr, "Connection established.\n");

      /* send message to client*/
      fprintf(stderr, "Sending the message...\n");
      if ((cc == write(ws, message, strlen(message))) == -1 ) {
        perror("write");
        exit(1);
      }
      fprintf(stderr, "Message sent.\n");

      /* Stop connection*/
      if (shutdown(ws, SHUT_RDWR) == -1) {
        perror("shutdown");
        exit(1);
      }

      /* close socket*/
      if (close(ws) == -1) {
        perror("close");
        exit(1);
      }
    }
}







