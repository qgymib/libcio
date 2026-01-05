#include "cio.h"

struct cio_rwlock
{
    SRWLOCK read_write_lock_; /**< Read-write lock. */
};

int cio_rwlock_init(cio_rwlock_t** rwlock)
{
    cio_rwlock_t* new_rwlock = cio_malloc(sizeof(cio_rwlock_t));
    if (new_rwlock == NULL)
    {
        return CIO_ENOMEM;
    }

    InitializeSRWLock(&new_rwlock->read_write_lock_);
    *rwlock = new_rwlock;
    return 0;
}

void cio_rwlock_exit(cio_rwlock_t* rwlock)
{
    cio_free(rwlock);
}

void cio_rwlock_rdlock(cio_rwlock_t* rwlock)
{
    AcquireSRWLockShared(&rwlock->read_write_lock_);
}

void cio_rwlock_rdunlock(cio_rwlock_t* rwlock)
{
    ReleaseSRWLockShared(&rwlock->read_write_lock_);
}

void cio_rwlock_wrlock(cio_rwlock_t* rwlock)
{
    AcquireSRWLockExclusive(&rwlock->read_write_lock_);
}

void cio_rwlock_wrunlock(cio_rwlock_t* rwlock)
{
    ReleaseSRWLockExclusive(&rwlock->read_write_lock_);
}
