#include "cio.h"
#include <process.h>

struct cio_thread
{
    HANDLE        thread; /**< Thread handle. */
    cio_thread_fn body;   /**< Thread entry point. */
    void*         arg;    /**< Thread argument. */
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
        break;
    }

    return CIO_EINVAL;
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
