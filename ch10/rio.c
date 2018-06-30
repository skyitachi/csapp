#include "rio.h"

/**
 * error_handle
 */
/*
ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  ssize_t total = 0;
  while(n) {
    ssize_t cnt = read(fd, usrbuf, n);
    if (!cnt) {
      return cnt;
    } else if (cnt < 0) {
      return -1;
    }
    usrbuf += cnt;
    n -= cnt;
    total += cnt;
  }
  return total;
}
*/

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
  while (nleft > 0) {
    if ((nread = read(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) {
        nread = 0;
      } else {
        return -1;
      }
    } else if (nread == 0) {
      break;
    }
    nleft -= nread;
    bufp += nread;
  }
  return (n - nleft);
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwrite;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nwrite = write(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) {
        nwrite = 0;
      } else {
        return -1;
      }
    }
    nleft -= nwrite;
    bufp += nwrite;
  }
  return n;
}

void rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;
  rp->rio_cnt = 0;
  rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
  int cnt;
  while(rp->rio_cnt <= 0) {
    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
    if (rp->rio_cnt < 0) {
      if (errno != EINTR) {
        return -1;
      }
    } else if (rp->rio_cnt == 0) {
      return 0;
    } else {
      rp->rio_bufptr = rp->rio_buf;
    }
  }
  cnt = n;
  if (rp->rio_cnt < n) {
    cnt = rp->rio_cnt;
  }
  memcpy(usrbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;
  return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  ssize_t n, rc;
  char c, *bufp = usrbuf;
  for(n = 1; n < maxlen; n++) {
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      *bufp++ = c;
      n++;
      if (c == '\n') {
        break;
      }
    } else if (rc == 0) {
      break;
    } else {
      return -1;
    }
  }
  *bufp = 0;
  return n - 1;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
  char *bufptr = usrbuf;
  ssize_t nleft = n;
  while(nleft > 0) {
    ssize_t rc = rio_read(rp, bufptr, nleft);
    if (rc == 0) {
      return n - nleft;
    } else if (rc < 0) {
      return -1;
    }
    nleft -= rc;
    bufptr += rc;
  }
  return n - nleft;
}

