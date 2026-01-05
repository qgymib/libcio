#define _GNU_SOURCE
#include "cio.h"
#include <assert.h>
#include <string.h>

#if defined(_WIN32)

////////////////////////////////////////////////////////////////////////////////
/// CIO_ERRNO-win32
////////////////////////////////////////////////////////////////////////////////

#include <process.h>


#else

////////////////////////////////////////////////////////////////////////////////
/// CIO_ERRNO-unix
////////////////////////////////////////////////////////////////////////////////

int cio_translate_sys_error(int sys_errno)
{
    return CIO_ERROR(sys_errno);
}

////////////////////////////////////////////////////////////////////////////////
/// CIO_MISC-unix
////////////////////////////////////////////////////////////////////////////////

uint64_t cio_hrtime(void)
{
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
    {
        abort();
    }
    return t.tv_sec * (uint64_t)1e9 + t.tv_nsec;
}

void cio_shutdown_library(void)
{
}

////////////////////////////////////////////////////////////////////////////////
/// CIO_THREAD-unix
////////////////////////////////////////////////////////////////////////////////

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
