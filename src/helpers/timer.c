#include "timer.h"

#include <time.h>

int timer(clock_t start) {
    return (clock() - start) * 1000 / CLOCKS_PER_SEC;
}