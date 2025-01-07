#include <cio.h>
#include "cutest.h"

typedef struct synchronization_thread_help
{
    int counter;
} synchronization_thread_help_t;

static void s_synchronization_thread_body(void* arg)
{
    synchronization_thread_help_t* helper = arg;

    for (int i = 0; i < 10; i++)
    {
        helper->counter++;
    }
}

TEST(Synchronization, thread)
{
    synchronization_thread_help_t* helper =
        cio_calloc(1, sizeof(synchronization_thread_help_t));
    ASSERT_NE_PTR(helper, NULL);

    cio_thread_t* thread = NULL;
    ASSERT_EQ_INT(
        cio_thread_init(&thread, s_synchronization_thread_body, helper), 0);
    ASSERT_NE_PTR(thread, NULL);

    ASSERT_EQ_INT(cio_thread_exit(thread, CIO_TIMEOUT_INFINITE), 0);
    ASSERT_EQ_INT(helper->counter, 10);

    cio_free(helper);
}

TEST(Synchronization, sleep)
{
    const uint64_t start_time = cio_hrtime();
    cio_thread_sleep(10);
    const uint64_t end_time = cio_hrtime();

    const uint64_t elapsed_time = end_time - start_time;
    ASSERT_GE_UINT64(elapsed_time, 10 * 1000 * 1000);
}
