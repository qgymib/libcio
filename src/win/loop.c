#include "cio.h"
#include "error.h"

struct cio_loop
{
    uint64_t time; /**< Current timestamp in milliseconds. */
    HANDLE   iocp; /**< IO completion port. */
};

int cio_loop_init(cio_loop_t** loop)
{
    cio_loop_t* new_loop = cio_calloc(1, sizeof(cio_loop_t));
    if (new_loop == NULL)
    {
        return CIO_ENOMEM;
    }

    cio_loop_update_time(new_loop);
    if ((new_loop->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
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

uint64_t cio_loop_now(const cio_loop_t* loop)
{
    return loop->time;
}

void cio_loop_update_time(cio_loop_t* loop)
{
    loop->time = cio_hrtime() / 1000000;
}