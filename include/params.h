#ifndef __PARAMS_H__
#define __PARAMS_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

#define NULL_TEST(ptr, line_num) \
    if (ptr == NULL) \
        {printf("error: %s:%d Null pointer exception\n", __FILE__, line_num); \
        exit(0);}

#define MALLOC_TEST(ptr, line_num) \
    if (ptr == NULL) \
        {printf("error: %s:%d malloc() failed\n", __FILE__, line_num); \
        exit(0);}

#define NUM_OF_USERS 2
#define NUM_OF_TXERS 10
#define NUM_OF_EDGES 10
#define NUM_OF_CDNS 1

#define BUFFER_LENGTH 64
#define LINK_CAPACITY 50

#define MTU 1500

#endif