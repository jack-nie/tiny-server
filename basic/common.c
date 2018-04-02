#include "common.h"

void unix_error(char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

ssize_t safe_read(int fd, void *buf, size_t n)
{
  ssize_t rc;
  if ((rc = read(fd, buf, n)) < 0)
    unix_error("read error");
  return rc;
}

ssize_t safe_write(int fd, const void *buf, size_t n)
{
  ssize_t rc;
  if ((rc = write(fd, buf, n)) < 0)
    unix_error("write error");
  return rc;
}

int safe_open(char *filename, int flags, mode_t mode)
{
  int rc;
  if ((rc = open(filename, flags, mode)) < 0)
    unix_error("open file error");
  return rc;
}

int safe_dup2(int fd1, int fd2)
{
  int rc;
  if ((rc = dup2(fd1, fd2)) < 0)
    unix_error("dup2 error");
  return rc;
}

int open_listenfd(int port)
{
  int listenfd, optval = 1;
  struct sockaddr_in serveraddr;
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)) < 0)
    return -1;
  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons((unsigned short) port);
  if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
    return -1;
  if (listen(listenfd, LISTENQ) < 0)
    return -1;
  return listenfd;
}
