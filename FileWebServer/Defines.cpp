#ifndef DEFINES
#define DEFINES

#include <stdio.h>

#define PrintLine(str, arg...) \
    printf("[%20s - %20s - %5d] ",__FILE__, __FUNCTION__, __LINE__); \
    printf(str, ##arg); \
    printf("\n")

#endif