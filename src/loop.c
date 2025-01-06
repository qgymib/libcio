#include "cio.h"

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

    new_loop->time = 0;

    *loop = new_loop;
    return 0;
}

int cio_loop_exit(cio_loop_t* loop)
{
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
