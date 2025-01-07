#include <cio.h>
#include "cutest.h"

static int s_synchronization_once_ret = 0;

static void s_on_once_callback(void)
{
    s_synchronization_once_ret++;
}

TEST(Synchronization, once)
{
    static cio_once_t token = CIO_ONCE_INIT;
    cio_once(&token, s_on_once_callback);
    cio_once(&token, s_on_once_callback);

    ASSERT_EQ_INT(s_synchronization_once_ret, 1);
}
