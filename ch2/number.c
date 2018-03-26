#include <stdio.h>

#define MIN -(1<<31)
int tadd_ok(int x, int y) {
  int sum = x + y;
  if (x > 0 && y > 0 && sum < 0) return 0;
  if (x < 0 && y < 0 && sum > 0) return 0;
  return 1;
}

int tsub_ok(int x, int y) {
  if (y == MIN && x > 0) return 0;
  if (y == MIN && x < 0) return 1;
  return tadd_ok(x, -y);
}

int main() {
  signed char a = 100;
  signed char b = 100;
  signed char c = a + b;
  printf("%d\n",  c);

  int a1 = - (1 << 31);
  printf("%d\n", -a1);

  printf("%d\n", tsub_ok(1, MIN)); // should 0
  printf("%d\n", tsub_ok(-1, MIN)); // should 1
}
