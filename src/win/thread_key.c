#include "cio.h"

struct cio_thread_key
{
    DWORD tls_index; /**< Thread local storage index. */
};

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
