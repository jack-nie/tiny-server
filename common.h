#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define	MAXLINE	 8192  /* max text line length */
#define MAXLEN 1024
#define RIO_BUFSIZE 8192
#define LISTENQ 1024

typedef struct {
  int rio_fd;
  int rio_cnt;
  char *rio_bufptr;
  char rio_buf[RIO_BUFSIZE];
} rio_t;

typedef struct sockaddr SA;

void unix_error(char *msg);
ssize_t safe_read(int fd, void *buf, size_t n);
ssize_t safe_write(int fd, const void *buf, size_t n);
int safe_open(char *filename, int flags, mode_t mode);
int safe_dup2(int fd1, int fd2);

/* Rio (robost io) package */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int n);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Client/server helper functions */
int open_clientfd(char *hostname, int portno);
int open_listenfd(int portno);
void echo(int connfd);
void command(void);
