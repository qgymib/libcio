#include "cio.h"

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
