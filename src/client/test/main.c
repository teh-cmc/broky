#include <stdlib.h>

int
main(void) {
    void *buf = malloc(42);
    (void)buf;

    /* valgrind will complain */
    return 0;
}
