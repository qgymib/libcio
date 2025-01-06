#include <stdlib.h>
#include <string.h>
#include "cio.h"

/**
 * @brief Builtin allocator function.
 */
static void* s_cio_builtin_allocator(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

/**
 * @brief Global allocator function.
 */
static cio_allocator_fn s_allocator = s_cio_builtin_allocator;

cio_allocator_fn cio_replace_allocator(cio_allocator_fn new_allocator)
{
    cio_allocator_fn old = s_allocator;
    s_allocator = new_allocator;
    return old;
}

void* cio_realloc(void* ptr, size_t size)
{
    return s_allocator(ptr, size);
}

void* cio_malloc(size_t size)
{
    return s_allocator(NULL, size);
}

void* cio_calloc(size_t nmemb, size_t size)
{
    const size_t nbytes = nmemb * size;
    void* p = s_allocator(NULL, nbytes);
    if (p != NULL)
    {
        memset(p, 0, nbytes);
    }
    return p;
}

void cio_free(void* ptr)
{
    s_allocator(ptr, 0);
}
