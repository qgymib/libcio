#define _GNU_SOURCE
#include "cio.h"
#include <assert.h>
#include <string.h>

#if defined(_WIN32)

////////////////////////////////////////////////////////////////////////////////
/// CIO_ERRNO-win32
////////////////////////////////////////////////////////////////////////////////

#include <process.h>

typedef struct err_pair
{
    int sys;
    int wrap;
} err_pair_t;

int cio_translate_sys_error(int sys_errno)
{
    static const err_pair_t errors[] = {
        { WSAEACCES,                        CIO_EACCES          },
        { ERROR_ELEVATION_REQUIRED,         CIO_EACCES          },
        { ERROR_CANT_ACCESS_FILE,           CIO_EACCES          },
        { ERROR_ADDRESS_ALREADY_ASSOCIATED, CIO_EADDRINUSE      },
        { WSAEADDRINUSE,                    CIO_EADDRINUSE      },
        { WSAEADDRNOTAVAIL,                 CIO_EADDRNOTAVAIL   },
        { WSAEAFNOSUPPORT,                  CIO_EAFNOSUPPORT    },
        { WSAEWOULDBLOCK,                   CIO_EAGAIN          },
        { ERROR_NO_DATA,                    CIO_EAGAIN          },
        { WSAEALREADY,                      CIO_EALREADY        },
        { ERROR_INVALID_FLAGS,              CIO_EBADF           },
        { ERROR_INVALID_HANDLE,             CIO_EBADF           },
        { ERROR_LOCK_VIOLATION,             CIO_EBUSY           },
        { ERROR_PIPE_BUSY,                  CIO_EBUSY           },
        { ERROR_SHARING_VIOLATION,          CIO_EBUSY           },
        { ERROR_OPERATION_ABORTED,          CIO_ECANCELED       },
        { WSAEINTR,                         CIO_ECANCELED       },
        { ERROR_CONNECTION_ABORTED,         CIO_ECONNABORTED    },
        { WSAECONNABORTED,                  CIO_ECONNABORTED    },
        { ERROR_CONNECTION_REFUSED,         CIO_ECONNREFUSED    },
        { WSAECONNREFUSED,                  CIO_ECONNREFUSED    },
        { ERROR_NETNAME_DELETED,            CIO_ECONNRESET      },
        { WSAECONNRESET,                    CIO_ECONNRESET      },
        { ERROR_ALREADY_EXISTS,             CIO_EEXIST          },
        { ERROR_FILE_EXISTS,                CIO_EEXIST          },
        { ERROR_NOACCESS,                   CIO_EFAULT          },
        { WSAEFAULT,                        CIO_EFAULT          },
        { ERROR_HOST_UNREACHABLE,           CIO_EHOSTUNREACH    },
        { WSAEHOSTUNREACH,                  CIO_EHOSTUNREACH    },
        { ERROR_INSUFFICIENT_BUFFER,        CIO_EINVAL          },
        { ERROR_INVALID_DATA,               CIO_EINVAL          },
        { ERROR_INVALID_PARAMETER,          CIO_EINVAL          },
        { ERROR_SYMLINK_NOT_SUPPORTED,      CIO_EINVAL          },
        { WSAEINVAL,                        CIO_EINVAL          },
        { WSAEPFNOSUPPORT,                  CIO_EINVAL          },
        { ERROR_BEGINNING_OF_MEDIA,         CIO_EIO             },
        { ERROR_BUS_RESET,                  CIO_EIO             },
        { ERROR_CRC,                        CIO_EIO             },
        { ERROR_DEVICE_DOOR_OPEN,           CIO_EIO             },
        { ERROR_DEVICE_REQUIRES_CLEANING,   CIO_EIO             },
        { ERROR_DISK_CORRUPT,               CIO_EIO             },
        { ERROR_EOM_OVERFLOW,               CIO_EIO             },
        { ERROR_FILEMARK_DETECTED,          CIO_EIO             },
        { ERROR_GEN_FAILURE,                CIO_EIO             },
        { ERROR_INVALID_BLOCK_LENGTH,       CIO_EIO             },
        { ERROR_IO_DEVICE,                  CIO_EIO             },
        { ERROR_NO_DATA_DETECTED,           CIO_EIO             },
        { ERROR_NO_SIGNAL_SENT,             CIO_EIO             },
        { ERROR_OPEN_FAILED,                CIO_EIO             },
        { ERROR_SETMARK_DETECTED,           CIO_EIO             },
        { ERROR_SIGNAL_REFUSED,             CIO_EIO             },
        { WSAEISCONN,                       CIO_EISCONN         },
        { ERROR_CANT_RESOLVE_FILENAME,      CIO_ELOOP           },
        { ERROR_TOO_MANY_OPEN_FILES,        CIO_EMFILE          },
        { WSAEMFILE,                        CIO_EMFILE          },
        { WSAEMSGSIZE,                      CIO_EMSGSIZE        },
        { ERROR_BUFFER_OVERFLOW,            CIO_ENAMETOOLONG    },
        { ERROR_FILENAME_EXCED_RANGE,       CIO_ENAMETOOLONG    },
        { ERROR_NETWORK_UNREACHABLE,        CIO_ENETUNREACH     },
        { WSAENETUNREACH,                   CIO_ENETUNREACH     },
        { WSAENOBUFS,                       CIO_ENOBUFS         },
        { ERROR_BAD_PATHNAME,               CIO_ENOENT          },
        { ERROR_DIRECTORY,                  CIO_ENOENT          },
        { ERROR_ENVVAR_NOT_FOUND,           CIO_ENOENT          },
        { ERROR_FILE_NOT_FOUND,             CIO_ENOENT          },
        { ERROR_INVALID_NAME,               CIO_ENOENT          },
        { ERROR_INVALID_DRIVE,              CIO_ENOENT          },
        { ERROR_INVALID_REPARSE_DATA,       CIO_ENOENT          },
        { ERROR_MOD_NOT_FOUND,              CIO_ENOENT          },
        { ERROR_PATH_NOT_FOUND,             CIO_ENOENT          },
        { WSAHOST_NOT_FOUND,                CIO_ENOENT          },
        { WSANO_DATA,                       CIO_ENOENT          },
        { ERROR_NOT_ENOUGH_MEMORY,          CIO_ENOMEM          },
        { ERROR_OUTOFMEMORY,                CIO_ENOMEM          },
        { ERROR_CANNOT_MAKE,                CIO_ENOSPC          },
        { ERROR_DISK_FULL,                  CIO_ENOSPC          },
        { ERROR_EA_TABLE_FULL,              CIO_ENOSPC          },
        { ERROR_END_OF_MEDIA,               CIO_ENOSPC          },
        { ERROR_HANDLE_DISK_FULL,           CIO_ENOSPC          },
        { ERROR_NOT_CONNECTED,              CIO_ENOTCONN        },
        { WSAENOTCONN,                      CIO_ENOTCONN        },
        { ERROR_DIR_NOT_EMPTY,              CIO_ENOTEMPTY       },
        { WSAENOTSOCK,                      CIO_ENOTSOCK        },
        { ERROR_NOT_SUPPORTED,              CIO_ENOTSUP         },
        { ERROR_BROKEN_PIPE,                CIO_EOF             },
        { ERROR_ACCESS_DENIED,              CIO_EPERM           },
        { ERROR_PRIVILEGE_NOT_HELD,         CIO_EPERM           },
        { ERROR_BAD_PIPE,                   CIO_EPIPE           },
        { ERROR_PIPE_NOT_CONNECTED,         CIO_EPIPE           },
        { WSAESHUTDOWN,                     CIO_EPIPE           },
        { WSAEPROTONOSUPPORT,               CIO_EPROTONOSUPPORT },
        { ERROR_WRITE_PROTECT,              CIO_EROFS           },
        { ERROR_SEM_TIMEOUT,                CIO_ETIMEDOUT       },
        { WSAETIMEDOUT,                     CIO_ETIMEDOUT       },
        { ERROR_NOT_SAME_DEVICE,            CIO_EXDEV           },
        { ERROR_INVALID_FUNCTION,           CIO_EISDIR          },
    };

    size_t i;
    if (sys_errno <= 0)
    {
        return sys_errno;
    }

    for (i = 0; i < CIO_ARRAY_SIZE(errors); i++)
    {
        const err_pair_t* p = &errors[i];
        if (p->sys == sys_errno)
        {
            return p->wrap;
        }
    }
    return CIO_UNKNOWN;
}

////////////////////////////////////////////////////////////////////////////////
/// CIO_LOOP-win32
////////////////////////////////////////////////////////////////////////////////

struct cio_loop
{
    uint64_t time; /**< Current timestamp in milliseconds. */
    HANDLE   iocp; /**< IO completion port. */
};

int cio_loop_init(cio_loop_t** loop)
{
    cio_loop_t* new_loop = cio_malloc(sizeof(cio_loop_t));
    if (new_loop == NULL)
    {
        return CIO_ENOMEM;
    }
    memset(new_loop, 0, sizeof(*new_loop));

    cio_loop_update_time(new_loop);
    if ((new_loop->iocp =
             CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
    {
        DWORD err = GetLastError();
        cio_free(new_loop);
        return cio_translate_sys_error((int)err);
    }

    *loop = new_loop;
    return 0;
}

int cio_loop_exit(cio_loop_t* loop)
{
    if (loop->iocp != NULL)
    {
        CloseHandle(loop->iocp);
        loop->iocp = NULL;
    }
    cio_free(loop);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// CIO_MISC-win32
////////////////////////////////////////////////////////////////////////////////

static uint64_t hrtime_frequency_ = 0;

static void s_cio_hrtime_init_once(void)
{
    LARGE_INTEGER perf_frequency;
    if (QueryPerformanceFrequency(&perf_frequency) == 0)
    {
        abort();
    }

    hrtime_frequency_ = perf_frequency.QuadPart;
}

uint64_t cio_hrtime(void)
{
    static cio_once_t token = CIO_ONCE_INIT;
    cio_once(&token, s_cio_hrtime_init_once);

    LARGE_INTEGER counter;
    if (QueryPerformanceCounter(&counter) == 0)
    {
        abort();
    }

    double scaled_freq = (double)hrtime_frequency_ / 1000000000;
    double result = (double)counter.QuadPart / scaled_freq;
    return (uint64_t)result;
}

void cio_shutdown_library(void)
{
}

////////////////////////////////////////////////////////////////////////////////
/// CIO_THREAD-win32
////////////////////////////////////////////////////////////////////////////////

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
    default:
        return CIO_EINVAL;
    }

    return CIO_EIO;
}

static BOOL CALLBACK s_init_once(PINIT_ONCE InitOnce, PVOID Parameter,
                                 PVOID* Context)
{
    (void)InitOnce;
    (void)Context;
    cio_once_fn fn = (cio_once_fn)Parameter;
    fn();
    return TRUE;
}

void cio_once(cio_once_t* guard, cio_once_fn cb)
{
    BOOL bStatus = InitOnceExecuteOnce(guard, s_init_once, (PVOID)cb, NULL);
    if (!bStatus)
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

////////////////////////////////////////////////////////////////////////////////
/// CIO_ERRNO-unix
////////////////////////////////////////////////////////////////////////////////

int cio_translate_sys_error(int sys_errno)
{
    return CIO_ERROR(sys_errno);
}

////////////////////////////////////////////////////////////////////////////////
/// CIO_LOOP-unix
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <semaphore.h>

struct cio_loop
{
    uint64_t time; /**< Current timestamp in milliseconds. */
};

int cio_loop_init(cio_loop_t** loop)
{
    cio_loop_t* new_loop = cio_malloc(sizeof(cio_loop_t));
    if (new_loop == NULL)
    {
        return CIO_ENOMEM;
    }
    memset(new_loop, 0, sizeof(*new_loop));
    cio_loop_update_time(new_loop);

    *loop = new_loop;
    return 0;
}

int cio_loop_exit(cio_loop_t* loop)
{
    cio_free(loop);
    return 0;
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

////////////////////////////////////////////////////////////////////////////////
/// CIO_ALLOCATOR
////////////////////////////////////////////////////////////////////////////////

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
    void*        p = s_allocator(NULL, nbytes);
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

////////////////////////////////////////////////////////////////////////////////
/// CIO_LOOP
////////////////////////////////////////////////////////////////////////////////

uint64_t cio_loop_now(const cio_loop_t* loop)
{
    return loop->time;
}

void cio_loop_update_time(cio_loop_t* loop)
{
    loop->time = cio_hrtime() / 1000000;
}
