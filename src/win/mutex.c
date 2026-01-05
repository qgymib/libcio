#include "cio.h"

struct cio_mutex
{
    CRITICAL_SECTION cs; /**< Critical section. */
};

int cio_mutex_init(cio_mutex_t** mutex, int recursive)
{
    (void)recursive;
    cio_mutex_t* new_mutex = cio_malloc(sizeof(cio_mutex_t));
    if (new_mutex == NULL)
    {
        return CIO_ENOMEM;
    }

    InitializeCriticalSection(&new_mutex->cs);
    *mutex = new_mutex;
    return 0;
}

void cio_mutex_exit(cio_mutex_t* mutex)
{
    DeleteCriticalSection(&mutex->cs);
    cio_free(mutex);
}

void cio_mutex_lock(cio_mutex_t* mutex)
{
    EnterCriticalSection(&mutex->cs);
}

void cio_mutex_unlock(cio_mutex_t* mutex)
{
    LeaveCriticalSection(&mutex->cs);
}
