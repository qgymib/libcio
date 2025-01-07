#define _GNU_SOURCE
#include "cio.h"
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

#if defined(_WIN32)

#include <process.h>

struct cio_thread
{
    HANDLE        thread; /**< Thread handle. */
    cio_thread_fn body;   /**< Thread entry point. */
    void*         arg;    /**< Thread argument. */
};

struct cio_thread_key
{
    DWORD tls_index; /**< Thread local storage index. */
};

struct cio_mutex
{
    CRITICAL_SECTION cs; /**< Critical section. */
};

struct cio_rwlock
{
    SRWLOCK read_write_lock_; /**< Read-write lock. */
};

struct cio_sem
{
    HANDLE sem; /**< Semaphore handle. */
};

static void s_once_iner_win(cio_once_t* guard, cio_once_fn cb)
{
    DWORD  result;
    HANDLE existing_event, created_event;

    created_event = CreateEvent(NULL, 1, 0, NULL);
    if (created_event == 0)
    {
        /* Could fail in a low-memory situation? */
        abort();
    }

    existing_event =
        InterlockedCompareExchangePointer(&guard->event, created_event, NULL);

    if (existing_event == NULL)
    {
        /* We won the race */
        cb();

        result = SetEvent(created_event);
        assert(result);
        guard->ran = 1;
    }
    else
    {
        /* We lost the race. Destroy the event we created and wait for the
         * existing one to become signaled. */
        CloseHandle(created_event);
        result = WaitForSingleObject(existing_event, INFINITE);
        assert(result == WAIT_OBJECT_0);
    }
}

static unsigned CALLBACK s_thread_fn_win(void* arg)
{
    cio_thread_t* thread = arg;
    cio_thread_fn body = thread->body;
    void*         data = thread->arg;

    body(data);
    return 0;
}

static int s_thread_init_win(cio_thread_t* thread, cio_thread_fn body,
                             void* arg)
{
    thread->body = body;
    thread->arg = arg;

    thread->thread = (HANDLE)_beginthreadex(NULL, 0, s_thread_fn_win, thread,
                                            CREATE_SUSPENDED, NULL);
    if (thread->thread != NULL)
    {
        ResumeThread(thread->thread);
        return 0;
    }

    int errcode = errno;
    switch (errcode)
    {
    case EACCES:
        return CIO_EACCES;
    case EAGAIN:
        return CIO_EAGAIN;
    case EINVAL:
        return CIO_EINVAL;
    }

    return CIO_EIO;
}

void cio_once(cio_once_t* guard, cio_once_fn cb)
{
    if (guard->ran)
    {
        return;
    }

    s_once_iner_win(guard, cb);
}

int cio_thread_init(cio_thread_t** thread, cio_thread_fn body, void* arg)
{
    cio_thread_t* new_thread = cio_malloc(sizeof(cio_thread_t));
    if (new_thread == NULL)
    {
        return CIO_ENOMEM;
    }

    int ret = s_thread_init_win(new_thread, body, arg);
    if (ret != 0)
    {
        cio_free(new_thread);
        return ret;
    }

    *thread = new_thread;
    return 0;
}

int cio_thread_exit(cio_thread_t* thread, uint32_t timeout)
{
    int ret = WaitForSingleObject(thread->thread, timeout);
    if (ret == WAIT_OBJECT_0)
    {
        CloseHandle(thread->thread);
        cio_free(thread);
        MemoryBarrier();
        return 0;
    }

    if (ret == WAIT_TIMEOUT)
    {
        return CIO_ETIMEDOUT;
    }

    abort();
}

void cio_thread_sleep(uint32_t ms)
{
    Sleep(ms);
}

int cio_thread_key_init(cio_thread_key_t** key)
{
    cio_thread_key_t* new_key = cio_malloc(sizeof(cio_thread_key_t));
    if (new_key == NULL)
    {
        return CIO_ENOMEM;
    }

    new_key->tls_index = TlsAlloc();
    if (new_key->tls_index == TLS_OUT_OF_INDEXES)
    {
        cio_free(new_key);
        return CIO_ENOMEM;
    }

    *key = new_key;
    return 0;
}

void cio_thread_key_exit(cio_thread_key_t* key)
{
    if (TlsFree(key->tls_index) == FALSE)
    {
        abort();
    }

    cio_free(key);
}

void cio_thread_key_set(cio_thread_key_t* key, void* value)
{
    if (TlsSetValue(key->tls_index, value) == FALSE)
    {
        abort();
    }
}

void* cio_thread_key_get(cio_thread_key_t* key)
{
    void* v = TlsGetValue(key->tls_index);
    if (v == NULL)
    {
        DWORD err = GetLastError();
        if (err != ERROR_SUCCESS)
        {
            abort();
        }
    }

    return v;
}

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

#else

#include <semaphore.h>

struct cio_thread
{
    pthread_t     thread; /**< Thread handle. */
    cio_thread_fn body;   /**< Thread entry point. */
    void*         arg;    /**< Thread argument. */
};

struct cio_thread_key
{
    pthread_key_t key; /**< Thread local storage key. */
};

struct cio_mutex
{
    pthread_mutex_t mutex; /**< Mutex handle. */
};

struct cio_rwlock
{
    pthread_rwlock_t rwlock; /**< Read-write lock handle. */
};

struct cio_sem
{
    sem_t sem; /**< Semaphore handle. */
};

static void* s_thread_fn_posix(void* arg)
{
    cio_thread_t* thread = arg;
    cio_thread_fn body = thread->body;
    void*         data = thread->arg;

    body(data);
    return NULL;
}

static int s_thread_init_posix(cio_thread_t* thread, cio_thread_fn body,
                               void* arg)
{
    thread->body = body;
    thread->arg = arg;

    int err = pthread_create(&thread->thread, NULL, s_thread_fn_posix, thread);

    return CIO_ERROR(err);
}

void cio_once(cio_once_t* guard, cio_once_fn cb)
{
    if (pthread_once(guard, cb))
    {
        abort();
    }
}

int cio_thread_init(cio_thread_t** thread, cio_thread_fn body, void* arg)
{
    cio_thread_t* new_thread = cio_malloc(sizeof(cio_thread_t));
    if (new_thread == NULL)
    {
        return CIO_ENOMEM;
    }

    int ret = s_thread_init_posix(new_thread, body, arg);
    if (ret != 0)
    {
        cio_free(new_thread);
        return ret;
    }

    *thread = new_thread;
    return 0;
}

int cio_thread_exit(cio_thread_t* thread, uint32_t timeout)
{
    if (timeout == (uint32_t)-1)
    {
        pthread_join(thread->thread, NULL);
        cio_free(thread);
        return 0;
    }

    while (timeout > 0)
    {
        uint32_t sleep_timeout = CIO_MIN(timeout, 10);

        int ret = pthread_tryjoin_np(thread->thread, NULL);
        if (ret == 0)
        {
            cio_free(thread);
            return 0;
        }

        if (ret == EBUSY)
        {
            cio_thread_sleep(sleep_timeout);
            timeout -= sleep_timeout;
            continue;
        }

        abort();
    }

    /* Try last time. */
    int ret = pthread_tryjoin_np(thread->thread, NULL);
    if (ret == 0)
    {
        cio_free(thread);
        return 0;
    }

    if (ret == EBUSY)
    {
        return CIO_ETIMEDOUT;
    }

    abort();
}

void cio_thread_sleep(uint32_t ms)
{
    struct timespec timeout;
    timeout.tv_sec = ms / 1000;
    timeout.tv_nsec = (ms % 1000) * 1000000;

    int rc;
    do
    {
        rc = nanosleep(&timeout, &timeout);
    } while (rc == -1 && errno == EINTR);

    assert(rc == 0);
}

int cio_thread_key_init(cio_thread_key_t** key)
{
    cio_thread_key_t* new_key = cio_malloc(sizeof(cio_thread_key_t));
    if (new_key == NULL)
    {
        return CIO_ENOMEM;
    }

    int ret = pthread_key_create(&new_key->key, NULL);
    if (ret != 0)
    {
        cio_free(new_key);
        return CIO_ERROR(ret);
    }

    *key = new_key;
    return 0;
}

void cio_thread_key_exit(cio_thread_key_t* key)
{
    if (pthread_key_delete(key->key) != 0)
    {
        abort();
    }

    cio_free(key);
}

void cio_thread_key_set(cio_thread_key_t* key, void* value)
{
    if (pthread_setspecific(key->key, value) != 0)
    {
        abort();
    }
}

void* cio_thread_key_get(cio_thread_key_t* key)
{
    return pthread_getspecific(key->key);
}

int cio_mutex_init(cio_mutex_t** mutex, int recursive)
{
    cio_mutex_t* new_mutex = cio_malloc(sizeof(cio_mutex_t));
    if (new_mutex == NULL)
    {
        return CIO_ENOMEM;
    }

    if (!recursive)
    {
        int ret = pthread_mutex_init(&new_mutex->mutex, NULL);
        if (ret != 0)
        {
            cio_free(new_mutex);
            return CIO_ERROR(ret);
        }

        *mutex = new_mutex;
        return 0;
    }

    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr) != 0)
    {
        abort();
    }
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
    {
        abort();
    }

    int ret = pthread_mutex_init(&new_mutex->mutex, &attr);
    if (pthread_mutexattr_destroy(&attr) != 0)
    {
        abort();
    }
    if (ret != 0)
    {
        cio_free(new_mutex);
        return CIO_ERROR(ret);
    }

    *mutex = new_mutex;
    return 0;
}

void cio_mutex_exit(cio_mutex_t* mutex)
{
    if (pthread_mutex_destroy(&mutex->mutex) != 0)
    {
        abort();
    }

    cio_free(mutex);
}

void cio_mutex_lock(cio_mutex_t* mutex)
{
    if (pthread_mutex_lock(&mutex->mutex) != 0)
    {
        abort();
    }
}

void cio_mutex_unlock(cio_mutex_t* mutex)
{
    if (pthread_mutex_unlock(&mutex->mutex) != 0)
    {
        abort();
    }
}

int cio_rwlock_init(cio_rwlock_t** rwlock)
{
    cio_rwlock_t* new_rwlock = cio_malloc(sizeof(cio_rwlock_t));
    if (new_rwlock == NULL)
    {
        return CIO_ENOMEM;
    }

    int ret = pthread_rwlock_init(&new_rwlock->rwlock, NULL);
    if (ret != 0)
    {
        cio_free(new_rwlock);
        return CIO_ERROR(ret);
    }

    *rwlock = new_rwlock;
    return 0;
}

void cio_rwlock_exit(cio_rwlock_t* rwlock)
{
    if (pthread_rwlock_destroy(&rwlock->rwlock) != 0)
    {
        abort();
    }

    cio_free(rwlock);
}

void cio_rwlock_rdlock(cio_rwlock_t* rwlock)
{
    if (pthread_rwlock_rdlock(&rwlock->rwlock) != 0)
    {
        abort();
    }
}

void cio_rwlock_rdunlock(cio_rwlock_t* rwlock)
{
    if (pthread_rwlock_unlock(&rwlock->rwlock) != 0)
    {
        abort();
    }
}

void cio_rwlock_wrlock(cio_rwlock_t* rwlock)
{
    if (pthread_rwlock_wrlock(&rwlock->rwlock) != 0)
    {
        abort();
    }
}

void cio_rwlock_wrunlock(cio_rwlock_t* rwlock)
{
    if (pthread_rwlock_unlock(&rwlock->rwlock) != 0)
    {
        abort();
    }
}

int cio_sem_init(cio_sem_t** sem, unsigned value)
{
    cio_sem_t* new_sem = cio_malloc(sizeof(cio_sem_t));
    if (new_sem == NULL)
    {
        return CIO_ENOMEM;
    }

    int ret = sem_init(&new_sem->sem, 0, value);
    if (ret != 0)
    {
        ret = errno;
        cio_free(new_sem);
        return CIO_ERROR(ret);
    }

    *sem = new_sem;
    return 0;
}

void cio_sem_exit(cio_sem_t* sem)
{
    if (sem_destroy(&sem->sem) != 0)
    {
        abort();
    }

    cio_free(sem);
}

void cio_sem_post(cio_sem_t* sem)
{
    if (sem_post(&sem->sem) != 0)
    {
        abort();
    }
}

int cio_sem_wait(cio_sem_t* sem, uint32_t timeout)
{
    while (timeout > 0)
    {
        uint32_t sleep_timeout = CIO_MIN(10, timeout);

        int ret = sem_trywait(&sem->sem);
        if (ret == 0)
        {
            return 0;
        }

        int err = errno;
        if (err == EAGAIN)
        {
            cio_thread_sleep(sleep_timeout);
            timeout -= sleep_timeout;
            continue;
        }
    }

    /* Try last time. */
    int ret = sem_wait(&sem->sem);
    if (ret == 0)
    {
        return 0;
    }

    return CIO_ETIMEDOUT;
}

#endif
