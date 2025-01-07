#include <cio.h>
#include "cutest.h"

typedef struct test_synchronization_thread_key_ctx
{
    cio_thread_t*     thread_1; /**< Test thread 1. */
    cio_thread_t*     thread_2; /**< Test thread 2. */
    cio_thread_key_t* key;      /**< Thread local storage. */
} test_synchronization_thread_key_ctx_t;

static test_synchronization_thread_key_ctx_t*
    s_test_synchronization_thread_key_ctx = NULL;

TEST_FIXTURE_SETUP(Synchronization)
{
    s_test_synchronization_thread_key_ctx =
        cio_calloc(1, sizeof(test_synchronization_thread_key_ctx_t));
    ASSERT_NE_PTR(s_test_synchronization_thread_key_ctx, NULL);
}

TEST_FIXTURE_TEARDOWN(Synchronization)
{
    cio_free(s_test_synchronization_thread_key_ctx);
    s_test_synchronization_thread_key_ctx = NULL;
}

static void s_test_synchronization_thread_key_ctx_thread_1(void* arg)
{
    cio_thread_key_set(s_test_synchronization_thread_key_ctx->key, arg);
}

static void s_test_synchronization_thread_key_ctx_thread_2(void* arg)
{
    void** p_arg = (void**)arg;
    *p_arg = cio_thread_key_get(s_test_synchronization_thread_key_ctx->key);
}

TEST_F(Synchronization, thread_key)
{
    ASSERT_EQ_INT(
        cio_thread_key_init(&s_test_synchronization_thread_key_ctx->key), 0);

    /*
     * Set first thread key to address of `a`.
     */
    {
        int a = 0;
        ASSERT_EQ_INT(
            cio_thread_init(&s_test_synchronization_thread_key_ctx->thread_1,
                            s_test_synchronization_thread_key_ctx_thread_1, &a),
            0);
        ASSERT_EQ_INT(
            cio_thread_exit(s_test_synchronization_thread_key_ctx->thread_1,
                            CIO_TIMEOUT_INFINITE),
            0);
    }

    /*
     * Get second thread key and verify.
     */
    {
        int   b;
        void* addr = &b;
        ASSERT_EQ_INT(
            cio_thread_init(&s_test_synchronization_thread_key_ctx->thread_2,
                            s_test_synchronization_thread_key_ctx_thread_2,
                            &addr),
            0);
        ASSERT_EQ_INT(
            cio_thread_exit(s_test_synchronization_thread_key_ctx->thread_2,
                            CIO_TIMEOUT_INFINITE),
            0);

        ASSERT_EQ_PTR(addr, NULL);
    }

    cio_thread_key_exit(s_test_synchronization_thread_key_ctx->key);
}
