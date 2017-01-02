#include "common.h"

int main(int argc, char **argv)
{
  int listenfd, connfd, port;
  socklen_t clientlen = sizeof(struct sockaddr);
  struct sockaddr_in clientaddr;
  fd_set read_set, ready_set;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  port = atoi(argv[0]);
  listenfd = open_listenfd(port);
  FD_ZERO(&read_set);
  FD_SET(STDIN_FILENO, &read_set);
  FD_SET(listenfd, &read_set);

  while (1)
  {
    ready_set = read_set;
    select(listenfd + 1, &ready_set, NULL, NULL, NULL);
    if (FD_ISSET(STDIN_FILENO, &ready_set))
      command();
    if (FD_ISSET(listenfd, &ready_set))
    {
      connfd = accept(listenfd, (SA *) &clientaddr, &clientlen);
      echo(connfd);
      close(connfd);
    }
  }
}

void echo(int connfd)
{
  size_t n;
  char buf[MAXLINE];
  rio_t rio;
  rio_readinitb(&rio, connfd);
  while ((n = rio_readlineb(&rio, buf, MAXLEN)) != 0)
  {
    printf("server received %d bytes\n", (int)n);
    rio_writen(connfd, buf, n);
  }
}

void command(void)
{
  char buf[MAXLEN];
  if (!fgets(buf,  MAXLEN, stdin))
    exit(0);
  printf("%s", buf);
}
