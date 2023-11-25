#ifndef __INCLUDE_OS_OS_MALLOC_H
#define __INCLUDE_OS_OS_MALLOC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <FreeRTOS.h>

#ifdef malloc
#undef malloc
#endif

#ifdef free
#undef free
#endif

#ifdef calloc
#undef calloc
#endif

#define malloc pvPortMalloc
#define free vPortFree
#define calloc(n, size) pvPortCalloc(n, size)

#endif /* __INCLUDE_OS_OS_MALLOC_H */