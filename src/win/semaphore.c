#include "cio.h"

struct cio_sem
{
    HANDLE sem; /**< Semaphore handle. */
};

int cio_sem_init(cio_sem_t** sem, unsigned value)
{
    cio_sem_t* new_sem = cio_malloc(sizeof(cio_sem_t));
    if (new_sem == NULL)
    {
        return CIO_ENOMEM;
    }

    new_sem->sem = CreateSemaphore(NULL, value, INT_MAX, NULL);
    if (new_sem->sem == NULL)
    {
        cio_free(new_sem);
        return CIO_ENOMEM;
    }

    *sem = new_sem;
    return 0;
}

void cio_sem_exit(cio_sem_t* sem)
{
    CloseHandle(sem->sem);
    cio_free(sem);
}

void cio_sem_post(cio_sem_t* sem)
{
    if (ReleaseSemaphore(sem->sem, 1, NULL) == 0)
    {
        abort();
    }
}

int cio_sem_wait(cio_sem_t* sem, uint32_t timeout)
{
    DWORD ret = WaitForSingleObject(sem->sem, timeout);
    if (ret == WAIT_OBJECT_0)
    {
        return 0;
    }
    else if (ret == WAIT_TIMEOUT)
    {
        return CIO_ETIMEDOUT;
    }
    abort();
}
