#include <stdlib.h>
#include <semaphore.h>
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
