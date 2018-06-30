#include "rio.h"
#include <time.h>

#define MAX_SIZE 128

char buf[MAX_SIZE];

int main() {
  rio_t rio_buf;
  int fd1 = open("bar.txt", O_RDONLY, 0);
  rio_readinitb(&rio_buf, fd1);
  ssize_t ret = rio_readnb(&rio_buf, buf, 100);
  printf("read count %zd\n", ret);
  printf("read content \n");
  printf("%s", buf);
  return 0;
}
