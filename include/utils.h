#ifndef ITD_UTILS_H
#define ITD_UTILS_H
#ifdef MAC
#include <stdint.h>
typedef uint32_t Uint32;
#endif
typedef enum{false=0, true=1}bool;

unsigned char int32toubyte8(int);
#endif
