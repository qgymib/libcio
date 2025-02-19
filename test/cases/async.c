#include <cio.h>
#include "cutest.h"

static int dummy_variable = 0;

TEST_FIXTURE_SETUP(async)
{
    dummy_variable = 0;
}

TEST_FIXTURE_TEARDOWN(async)
{
}

static void s_on_async(void* arg)
{
    ASSERT_EQ_PTR(arg, &dummy_variable);
    dummy_variable++;
}

static void s_on_exit(void* arg)
{
    ASSERT_EQ_PTR(arg, &dummy_variable);
    dummy_variable++;
}

TEST_F(async, async)
{
    cio_loop_t* loop = NULL;
    ASSERT_EQ_INT(cio_loop_init(&loop), 0);
    ASSERT_NE_PTR(loop, NULL);

    cio_async_t* async = NULL;
    ASSERT_EQ_INT(cio_async_init(&async, loop, s_on_async, &dummy_variable), 0);
    ASSERT_NE_PTR(async, NULL);

    cio_async_send(async);
    ASSERT_EQ_INT(cio_loop_run(loop, CIO_LOOP_ONCE, CIO_TIMEOUT_INFINITE), 0);
    ASSERT_EQ_INT(dummy_variable, 1);

    cio_async_exit(async, s_on_exit, &dummy_variable);
    ASSERT_EQ_INT(cio_loop_run(loop, CIO_LOOP_DEFAULT, CIO_TIMEOUT_INFINITE),
                  0);
    ASSERT_EQ_INT(dummy_variable, 2);

    ASSERT_EQ_INT(cio_loop_exit(loop), 0);
}
