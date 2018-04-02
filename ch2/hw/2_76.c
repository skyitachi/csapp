#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *calloc(size_t nmemb, size_t size) {
    if (!nmemb || !size) {
        return NULL;
    }
    size_t total = nmemb * size;
    // 乘法溢出的检测
    if (total / nmemb != size) {
        return NULL;
    }
    void *ans = malloc(total);
    memset(ans, 0, total);
    return ans;
}

int main() {
    void *ans = calloc(0x7fffffffff, 0x7fffffffff);
    printf("allocate: %p\n", ans);
}
