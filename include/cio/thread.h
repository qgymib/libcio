#ifndef CIO_THREAD_H
#define CIO_THREAD_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup CIO_SYNCHRONIZATION Threading and synchronization utilities
 * @{
 */

#if defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <Windows.h>

/**
 * @brief Once execution guard.
 */
typedef struct cio_once
{
    unsigned char ran;
    HANDLE        event;
} cio_once_t;

/**
 * @brief Static initializer for #cio_once_t.
 */
#define CIO_ONCE_INIT { 0, NULL }

#else

#include <pthread.h>

/**
 * @brief Once execution guard.
 */
typedef pthread_once_t cio_once_t;

/**
 * @brief Static initializer for #cio_once_t.
 */
#define CIO_ONCE_INIT PTHREAD_ONCE_INIT

#endif

/**
 * @defgroup CIO_ONCE Once-only initialization
 * @{
 */

/**
 * @brief Once execution callback.
 */
typedef void (*cio_once_fn)(void);

/**
 * @brief Runs a function once and only once.
 * @param[in,out] guard Once guard. Must be initialized statically by
 *   #CIO_ONCE_INIT.
 * @param[in] cb Function callback.
 * @see #CIO_ONCE_INIT
 */
void cio_once(cio_once_t* guard, cio_once_fn cb);

/**
 * @}
 */

/**
 * @defgroup CIO_THREAD Threads
 * @{
 */

/**
 * @brief Thread handle.
 */
typedef struct cio_thread cio_thread_t;

/**
 * @brief Thread entry point.
 * @param[in] arg Thread argument.
 */
typedef void (*cio_thread_fn)(void* arg);

/**
 * @brief Create a new thread.
 * @param[out] thread Thread handle.
 * @param[in] body Thread entry point.
 * @param[in] arg Thread argument.
 * @return 0 if create success.
 * @return #CIO_ENOMEM if out of memory.
 */
int cio_thread_init(cio_thread_t** thread, cio_thread_fn body, void* arg);

/**
 * @brief Wait for thread to terminate.
 * @param[in] thread Thread handle.
 * @param[in] timeout Timeout in milliseconds.
 * @return 0 if thread terminated.
 * @return #CIO_ETIMEDOUT if timeout.
 */
int cio_thread_exit(cio_thread_t* thread, uint32_t timeout);

/**
 * @brief Sleep for a specified number of milliseconds.
 * @param[in] ms Number of milliseconds to sleep.
 */
void cio_thread_sleep(uint32_t ms);

/**
 * @}
 */

/**
 * @defgroup CIO_THREAD_KEY Thread-local storage
 * @{
 */

/**
 * @brief Thread-local storage key.
 */
typedef struct cio_thread_key cio_thread_key_t;

/**
 * @brief Create a new thread-local storage key.
 * @param[out] key Thread-local storage key.
 * @return 0 if success.
 * @return #CIO_ENOMEM if out of memory.
 */
int cio_thread_key_init(cio_thread_key_t** key);

/**
 * @brief Delete a thread-local storage key.
 * @param[in] key Thread-local storage key.
 */
void cio_thread_key_exit(cio_thread_key_t* key);

/**
 * @brief Set the value of a thread-local storage key.
 * @param[in] key Thread-local storage key.
 * @param[in] value Thread-local storage value.
 */
void cio_thread_key_set(cio_thread_key_t* key, void* value);

/**
 * @brief Get the value of a thread-local storage key.
 * @param[in] key Thread-local storage key.
 * @return Thread-local storage value.
 */
void* cio_thread_key_get(cio_thread_key_t* key);

/**
 * @}
 */

/**
 * @defgroup CIO_MUTEX Mutex locks
 * @{
 */

/**
 * @brief Mutex handle.
 */
typedef struct cio_mutex cio_mutex_t;

/**
 * @brief Create a new mutex.
 * @param[out] mutex Mutex handle.
 * @param[in] recursive Whether the mutex is recursive.
 * @return 0 if success.
 * @return #CIO_ENOMEM if out of memory.
 */
int cio_mutex_init(cio_mutex_t** mutex, int recursive);

/**
 * @brief Delete a mutex.
 * @param[in] mutex Mutex handle.
 */
void cio_mutex_exit(cio_mutex_t* mutex);

/**
 * @brief Lock a mutex.
 * @param[in] mutex Mutex handle.
 */
void cio_mutex_lock(cio_mutex_t* mutex);

/**
 * @brief Unlock a mutex.
 * @param[in] mutex Mutex handle.
 */
void cio_mutex_unlock(cio_mutex_t* mutex);

/**
 * @}
 */

/**
 * @defgroup CIO_RWLOCK Read-write locks
 * @{
 */

/**
 * @brief Read-write lock handle.
 */
typedef struct cio_rwlock cio_rwlock_t;

/**
 * @brief Create a new read-write lock.
 * @param[out] rwlock Read-write lock handle.
 * @return 0 if success.
 * @return #CIO_ENOMEM if out of memory.
 */
int cio_rwlock_init(cio_rwlock_t** rwlock);

/**
 * @brief Delete a read-write lock.
 * @param[in] rwlock Read-write lock handle.
 */
void cio_rwlock_exit(cio_rwlock_t* rwlock);

/**
 * @brief Lock a read-write lock for reading.
 * @param[in] rwlock Read-write lock handle.
 */
void cio_rwlock_rdlock(cio_rwlock_t* rwlock);

/**
 * @brief Unlock a read-write lock for reading.
 * @param[in] rwlock Read-write lock handle.
 */
void cio_rwlock_rdunlock(cio_rwlock_t* rwlock);

/**
 * @brief Lock a read-write lock for writing.
 * @param[in] rwlock Read-write lock handle.
 */
void cio_rwlock_wrlock(cio_rwlock_t* rwlock);

/**
 * @brief Unlock a read-write lock for writing.
 * @param[in] rwlock Read-write lock handle.
 */
void cio_rwlock_wrunlock(cio_rwlock_t* rwlock);

/**
 * @}
 */

/**
 * @defgroup CIO_SEMAPHORE Semaphores
 * @{
 */

/**
 * @brief Semaphore handle.
 */
typedef struct cio_sem cio_sem_t;

/**
 * @brief Create a new semaphore.
 * @param[out] sem Semaphore handle.
 * @param[in] value Initial value.
 * @return 0 if success.
 * @return #CIO_ENOMEM if out of memory.
 */
int cio_sem_init(cio_sem_t** sem, unsigned value);

/**
 * @brief Delete a semaphore.
 * @param[in] sem Semaphore handle.
 */
void cio_sem_exit(cio_sem_t* sem);

/**
 * @brief Post a semaphore.
 * @param[in] sem Semaphore handle.
 */
void cio_sem_post(cio_sem_t* sem);

/**
 * @brief Wait for a semaphore.
 * @param[in] sem Semaphore handle.
 * @param[in] timeout Timeout in milliseconds.
 * @return 0 if success.
 * @return #CIO_ETIMEDOUT if timeout.
 */
int cio_sem_wait(cio_sem_t* sem, uint32_t timeout);

/**
 * @}
 */

/**
 * @} // CIO_SYNCHRONIZATION
 */

#ifdef __cplusplus
}
#endif
#endif // CIO_THREAD_H
