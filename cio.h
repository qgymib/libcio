#ifndef CIO_H
#define CIO_H

#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0600
#endif

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup CIO_ALLOCATOR Memory management
 * @{
 */

/**
 * @brief Changes the size of the memory block pointed to by \p ptr to \p size
 *   bytes.
 * @see https://man7.org/linux/man-pages/man3/realloc.3p.html
 */
typedef void* (*cio_allocator_fn)(void* ptr, size_t size);

/**
 * @brief Replace allocator with new function.
 * @warning If you want to replace allocator, this must be the first function to
 * call.
 * @param[in] new_allocator New allocator function.
 * @return Old allocator.
 */
cio_allocator_fn cio_replace_allocator(cio_allocator_fn new_allocator);

/**
 * @brief Change the size of the memory block.
 * @see https://man7.org/linux/man-pages/man3/free.3.html
 * @param[in] ptr The memory block to change.
 * @param[in] size The new size.
 * @return New address.
 */
void* cio_realloc(void* ptr, size_t size);

/**
 * @brief Allocate memory.
 * @see https://man7.org/linux/man-pages/man3/free.3.html
 * @param[in] size Size of bytes to alloc.
 * @return A pointer to the allocated memory.
 */
void* cio_malloc(size_t size);

/**
 * @brief Allocates memory for an array of \p nmemb elements of \p size bytes
 *   each and returns a pointer to the allocated memory.
 * @see https://man7.org/linux/man-pages/man3/free.3.html
 * @param nmemb The number of elements.
 * @param size The size of each element.
 * @return The allocated address.
 */
void* cio_calloc(size_t nmemb, size_t size);

/**
 * @brief Free memory space.
 * @see https://man7.org/linux/man-pages/man3/free.3.html
 * @param[in] ptr The memory space to free
 */
void cio_free(void* ptr);

/**
 * @} // GROUP: CIO_ALLOCATOR
 */

/**
 * @defgroup CIO_ERRNO Error Error handling
 * @{
 */

#if EDOM > 0
#define CIO_ERROR(x) (-(x))
#else
#define CIO_ERROR(x) (x)
#endif

#if defined(EPERM) && !defined(_WIN32)
#define CIO__EPERM CIO_ERROR(EPERM)
#else
#define CIO__EPERM (-4093)
#endif

#if defined(ENOENT) && !defined(_WIN32)
#define CIO__ENOENT CIO_ERROR(ENOENT)
#else
#define CIO__ENOENT (-4092)
#endif

#if defined(EIO) && !defined(_WIN32)
#define CIO__EIO CIO_ERROR(EIO)
#else
#define CIO__EIO (-4089)
#endif

#if defined(E2BIG) && !defined(_WIN32)
#define CIO__E2BIG CIO_ERROR(E2BIG)
#else
#define CIO__E2BIG (-4087)
#endif

#if defined(EBADF) && !defined(_WIN32)
#define CIO__EBADF CIO_ERROR(EBADF)
#else
#define CIO__EBADF (-4085)
#endif

#if defined(EAGAIN) && !defined(_WIN32)
#define CIO__EAGAIN CIO_ERROR(EAGAIN)
#else
#define CIO__EAGAIN (-4083)
#endif

#if defined(ENOMEM) && !defined(_WIN32)
#define CIO__ENOMEM CIO_ERROR(ENOMEM)
#else
#define CIO__ENOMEM (-4082)
#endif

#if defined(EACCES) && !defined(_WIN32)
#define CIO__EACCES CIO_ERROR(EACCES)
#else
#define CIO__EACCES (-4081)
#endif

#if defined(EFAULT) && !defined(_WIN32)
#define CIO__EFAULT CIO_ERROR(EFAULT)
#else
#define CIO__EFAULT (-4080)
#endif

#if defined(EBUSY) && !defined(_WIN32)
#define CIO__EBUSY CIO_ERROR(EBUSY)
#else
#define CIO__EBUSY (-4078)
#endif

#if defined(EEXIST) && !defined(_WIN32)
#define CIO__EEXIST CIO_ERROR(EEXIST)
#else
#define CIO__EEXIST (-4077)
#endif

#if defined(EXDEV) && !defined(_WIN32)
#define CIO__EXDEV CIO_ERROR(EXDEV)
#else
#define CIO__EXDEV (-4076)
#endif

#if defined(ENOTDIR) && !defined(_WIN32)
#define CIO__ENOTDIR CIO_ERROR(ENOTDIR)
#else
#define CIO__ENOTDIR (-4074)
#endif

#if defined(EISDIR) && !defined(_WIN32)
#define CIO__EISDIR CIO_ERROR(EISDIR)
#else
#define CIO__EISDIR (-4073)
#endif

#if defined(EINVAL) && !defined(_WIN32)
#define CIO__EINVAL CIO_ERROR(EINVAL)
#else
#define CIO__EINVAL (-4072)
#endif

#if defined(ENFILE) && !defined(_WIN32)
#define CIO__ENFILE CIO_ERROR(ENFILE)
#else
#define CIO__ENFILE (-4071)
#endif

#if defined(EMFILE) && !defined(_WIN32)
#define CIO__EMFILE CIO_ERROR(EMFILE)
#else
#define CIO__EMFILE (-4070)
#endif

#if defined(ENOSPC) && !defined(_WIN32)
#define CIO__ENOSPC CIO_ERROR(ENOSPC)
#else
#define CIO__ENOSPC (-4066)
#endif

#if defined(EROFS) && !defined(_WIN32)
#define CIO__EROFS CIO_ERROR(EROFS)
#else
#define CIO__EROFS (-4064)
#endif

#if defined(EPIPE) && !defined(_WIN32)
#define CIO__EPIPE CIO_ERROR(EPIPE)
#else
#define CIO__EPIPE (-4062)
#endif

#if defined(ENAMETOOLONG) && !defined(_WIN32)
#define CIO__ENAMETOOLONG CIO_ERROR(ENAMETOOLONG)
#else
#define CIO__ENAMETOOLONG (-4058)
#endif

#if defined(ENOSYS) && !defined(_WIN32)
#define CIO__ENOSYS CIO_ERROR(ENOSYS)
#else
#define CIO__ENOSYS (-4056)
#endif

#if defined(ENOTEMPTY) && !defined(_WIN32)
#define CIO__ENOTEMPTY CIO_ERROR(ENOTEMPTY)
#else
#define CIO__ENOTEMPTY (-4055)
#endif

#if defined(ELOOP) && !defined(_WIN32)
#define CIO__ELOOP CIO_ERROR(ELOOP)
#else
#define CIO__ELOOP (-4054)
#endif

#if defined(EPROTO) && !defined(_WIN32)
#define CIO__EPROTO CIO_ERROR(EPROTO)
#else
#define CIO__EPROTO (-4023)
#endif

#if defined(ENOTSOCK) && !defined(_WIN32)
#define CIO__ENOTSOCK CIO_ERROR(ENOTSOCK)
#else
#define CIO__ENOTSOCK (-4006)
#endif

#if defined(EMSGSIZE) && !defined(_WIN32)
#define CIO__EMSGSIZE CIO_ERROR(EMSGSIZE)
#else
#define CIO__EMSGSIZE (-4004)
#endif

#if defined(EPROTONOSUPPORT) && !defined(_WIN32)
#define CIO__EPROTONOSUPPORT CIO_ERROR(EPROTONOSUPPORT)
#else
#define CIO__EPROTONOSUPPORT (-4001)
#endif

#if defined(ENOTSUP) && !defined(_WIN32)
#define CIO__ENOTSUP CIO_ERROR(ENOTSUP)
#else
#define CIO__ENOTSUP (-3999)
#endif

#if defined(EAFNOSUPPORT) && !defined(_WIN32)
#define CIO__EAFNOSUPPORT CIO_ERROR(EAFNOSUPPORT)
#else
#define CIO__EAFNOSUPPORT (-3997)
#endif

#if defined(EADDRINUSE) && !defined(_WIN32)
#define CIO__EADDRINUSE CIO_ERROR(EADDRINUSE)
#else
#define CIO__EADDRINUSE (-3996)
#endif

#if defined(EADDRNOTAVAIL) && !defined(_WIN32)
#define CIO__EADDRNOTAVAIL CIO_ERROR(EADDRNOTAVAIL)
#else
#define CIO__EADDRNOTAVAIL (-3995)
#endif

#if defined(ENETUNREACH) && !defined(_WIN32)
#define CIO__ENETUNREACH CIO_ERROR(ENETUNREACH)
#else
#define CIO__ENETUNREACH (-3993)
#endif

#if defined(ECONNABORTED) && !defined(_WIN32)
#define CIO__ECONNABORTED CIO_ERROR(ECONNABORTED)
#else
#define CIO__ECONNABORTED (-3991)
#endif

#if defined(ECONNRESET) && !defined(_WIN32)
#define CIO__ECONNRESET CIO_ERROR(ECONNRESET)
#else
#define CIO__ECONNRESET (-3990)
#endif

#if defined(ENOBUFS) && !defined(_WIN32)
#define CIO__ENOBUFS CIO_ERROR(ENOBUFS)
#else
#define CIO__ENOBUFS (-3989)
#endif

#if defined(EISCONN) && !defined(_WIN32)
#define CIO__EISCONN CIO_ERROR(EISCONN)
#else
#define CIO__EISCONN (-3988)
#endif

#if defined(ENOTCONN) && !defined(_WIN32)
#define CIO__ENOTCONN CIO_ERROR(ENOTCONN)
#else
#define CIO__ENOTCONN (-3987)
#endif

#if defined(ETIMEDOUT) && !defined(_WIN32)
#define CIO__ETIMEDOUT CIO_ERROR(ETIMEDOUT)
#else
#define CIO__ETIMEDOUT (-3984)
#endif

#if defined(ECONNREFUSED) && !defined(_WIN32)
#define CIO__ECONNREFUSED CIO_ERROR(ECONNREFUSED)
#else
#define CIO__ECONNREFUSED (-3983)
#endif

#if defined(EHOSTUNREACH) && !defined(_WIN32)
#define CIO__EHOSTUNREACH CIO_ERROR(EHOSTUNREACH)
#else
#define CIO__EHOSTUNREACH (-3981)
#endif

#if defined(EALREADY) && !defined(_WIN32)
#define CIO__EALREADY CIO_ERROR(EALREADY)
#else
#define CIO__EALREADY (-3980)
#endif

#if defined(EINPROGRESS) && !defined(_WIN32)
#define CIO__EINPROGRESS CIO_ERROR(EINPROGRESS)
#else
#define CIO__EINPROGRESS (-3979)
#endif

#if defined(ECANCELED) && !defined(_WIN32)
#define CIO__ECANCELED CIO_ERROR(ECANCELED)
#else
#define CIO__ECANCELED (-3969)
#endif

typedef enum cio_errno
{
    CIO_EPERM = CIO__EPERM,
    CIO_ENOENT = CIO__ENOENT,
    CIO_EIO = CIO__EIO,
    CIO_E2BIG = CIO__E2BIG,
    CIO_EBADF = CIO__EBADF,
    CIO_EAGAIN = CIO__EAGAIN,
    CIO_ENOMEM = CIO__ENOMEM,
    CIO_EACCES = CIO__EACCES,
    CIO_EFAULT = CIO__EFAULT,
    CIO_EBUSY = CIO__EBUSY,
    CIO_EEXIST = CIO__EEXIST,
    CIO_EXDEV = CIO__EXDEV,
    CIO_ENOTDIR = CIO__ENOTDIR,
    CIO_EISDIR = CIO__EISDIR,
    CIO_EINVAL = CIO__EINVAL,
    CIO_ENFILE = CIO__ENFILE,
    CIO_EMFILE = CIO__EMFILE,
    CIO_ENOSPC = CIO__ENOSPC,
    CIO_EROFS = CIO__EROFS,
    CIO_EPIPE = CIO__EPIPE,
    CIO_ENAMETOOLONG = CIO__ENAMETOOLONG,
    CIO_ENOSYS = CIO__ENOSYS,
    CIO_ENOTEMPTY = CIO__ENOTEMPTY,
    CIO_ELOOP = CIO__ELOOP,
    CIO_EPROTO = CIO__EPROTO,
    CIO_ENOTSOCK = CIO__ENOTSOCK,
    CIO_EMSGSIZE = CIO__EMSGSIZE,
    CIO_EPROTONOSUPPORT = CIO__EPROTONOSUPPORT,
    CIO_ENOTSUP = CIO__ENOTSUP,
    CIO_EAFNOSUPPORT = CIO__EAFNOSUPPORT,
    CIO_EADDRINUSE = CIO__EADDRINUSE,
    CIO_EADDRNOTAVAIL = CIO__EADDRNOTAVAIL,
    CIO_ENETUNREACH = CIO__ENETUNREACH,
    CIO_ECONNABORTED = CIO__ECONNABORTED,
    CIO_ECONNRESET = CIO__ECONNRESET,
    CIO_ENOBUFS = CIO__ENOBUFS,
    CIO_EISCONN = CIO__EISCONN,
    CIO_ENOTCONN = CIO__ENOTCONN,
    CIO_ETIMEDOUT = CIO__ETIMEDOUT,
    CIO_ECONNREFUSED = CIO__ECONNREFUSED,
    CIO_EHOSTUNREACH = CIO__EHOSTUNREACH,
    CIO_EALREADY = CIO__EALREADY,
    CIO_EINPROGRESS = CIO__EINPROGRESS,
    CIO_ECANCELED = CIO__ECANCELED,

    CIO_EOF = -4095,
    CIO_UNKNOWN = -4094,
} cio_errno_t;

int cio_translate_sys_error(int sys_errno);

/**
 * @} GROUP: CIO_ERRNO
 */

/**
 * @defgroup CIO_SYNCHRONIZATION Threading and synchronization utilities
 * @{
 */

/**
 * @defgroup CIO_ONCE Once-only initialization
 * @{
 */

#if defined(_WIN32)

#include <Windows.h>

/**
 * @brief Once execution guard.
 */
typedef INIT_ONCE cio_once_t;

/**
 * @brief Static initializer for #cio_once_t.
 */
#define CIO_ONCE_INIT INIT_ONCE_STATIC_INIT

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

/**
 * @defgroup CIO_LOOP Event loop
 * @{
 */

/**
 * @brief Event loop handle.
 */
typedef struct cio_loop cio_loop_t;

/**
 * @brief Create a new event loop.
 * @param[out] loop Event loop handle.
 * @return 0 if create success.
 */
int cio_loop_init(cio_loop_t** loop);

/**
 * @brief Releases all internal loop resources.
 * @param[in] loop Release an event loop.
 * @return 0 if release success.
 * @return #CIO_EBUSY if there are any pending requests or open handles.
 */
int cio_loop_exit(cio_loop_t* loop);

/**
 * @brief Run event loop.
 * @param[in] loop Event loop
 * @param[in] mode Run mode
 * @param[in] timeout Timeout in milliseconds
 * @return 0
 */
int cio_loop_run(cio_loop_t* loop, int mode, uint32_t timeout);

/**
 * @brief Get the current timestamp in milliseconds.
 * @param[in] loop The event loop.
 * @return Current timestamp in milliseconds.
 */
uint64_t cio_loop_now(const cio_loop_t* loop);

/**
 * @brief Updates the current timestamp.
 * @param[in] loop The event loop.
 */
void cio_loop_update_time(cio_loop_t* loop);

/**
 * @}
 */

/**
 * @defgroup CIO_THREADPOOL Thread pool work scheduling
 * @{
 */

typedef void (*cio_work_cb)(void* arg);
typedef void (*cio_after_work_cb)(void* arg);

/**
 * @brief Post a work request, which run \p work in threadpool and run \p
 * after_work in \p loop.
 * @param[in] loop Event loop
 * @param[in] work Work callback
 * @param[in] after_work After work callback
 * @param[in] arg User defined argument passed to \p work and \p after_work.
 * @return <0 if failure, or a unique ID.
 */
int64_t cio_queue_work(cio_loop_t* loop, cio_work_cb work,
                       cio_after_work_cb after_work, void* arg);

/**
 * @brief Cancel a work.
 * @param[in] workid Work ID.
 * @return 0 if success.
 */
int cio_cancel(int64_t workid);

/**
 * @}
 */

/**
 * @defgroup CIO_LIST List
 * @{
 */

/**
 * @brief Static initializer for #cio_list_t
 * @see cio_list_t
 */
#define CIO_LIST_INIT { NULL, NULL, 0 }

/**
 * @brief Static initializer for #cio_list_node_t
 * @see cio_list_node_t
 */
#define CIO_LIST_NODE_INIT { NULL, NULL }

/**
 * @brief The list node.
 * This node must put in your struct.
 * @see EV_LIST_NODE_INIT
 */
typedef struct cio_list_node
{
    struct cio_list_node* p_after;  /**< Pointer to next node */
    struct cio_list_node* p_before; /**< Pointer to previous node */
} cio_list_node_t;

/**
 * @brief Double Linked List
 * @see EV_LIST_INIT
 */
typedef struct cio_list
{
    cio_list_node_t* head; /**< Pointer to HEAD node */
    cio_list_node_t* tail; /**< Pointer to TAIL node */
    size_t           size; /**< The number of total nodes */
} cio_list_t;

/**
 * @brief Initialize Double Linked List.
 * @note It is guarantee that memset() to zero have the same affect.
 * @param[out] handler  Pointer to list
 */
void cio_list_init(cio_list_t* handler);

/**
 * @brief Insert a node to the head of the list.
 * @warning the node must not exist in any list.
 * @param[in,out] handler   Pointer to list
 * @param[in,out] node      Pointer to a new node
 */
void cio_list_push_front(cio_list_t* handler, cio_list_node_t* node);

/**
 * @brief Insert a node to the tail of the list.
 * @warning the node must not exist in any list.
 * @param[in,out] handler   Pointer to list
 * @param[in,out] node      Pointer to a new node
 */
void cio_list_push_back(cio_list_t* handler, cio_list_node_t* node);

/**
 * @brief Insert a node in front of a given node.
 * @warning the node must not exist in any list.
 * @param[in,out] handler   Pointer to list
 * @param[in,out] pos       Pointer to a exist node
 * @param[in,out] node      Pointer to a new node
 */
void cio_list_insert_before(cio_list_t* handler, cio_list_node_t* pos,
                            cio_list_node_t* node);

/**
 * @brief Insert a node right after a given node.
 * @warning the node must not exist in any list.
 * @param[in,out] handler   Pointer to list
 * @param[in,out] pos       Pointer to a exist node
 * @param[in,out] node      Pointer to a new node
 */
void cio_list_insert_after(cio_list_t* handler, cio_list_node_t* pos,
                           cio_list_node_t* node);

/**
 * @brief Delete a exist node
 * @warning The node must already in the list.
 * @param[in,out] handler   Pointer to list
 * @param[in,out] node      The node you want to delete
 */
void cio_list_erase(cio_list_t* handler, cio_list_node_t* node);

/**
 * @brief Get the number of nodes in the list.
 * @param[in] handler   Pointer to list
 * @return              The number of nodes
 */
size_t cio_list_size(const cio_list_t* handler);

/**
 * @brief Get the first node and remove it from the list.
 * @param[in,out] handler   Pointer to list
 * @return                  The first node
 */
cio_list_node_t* cio_list_pop_front(cio_list_t* handler);

/**
 * @brief Get the last node and remove it from the list.
 * @param[in,out] handler   Pointer to list
 * @return                  The last node
 */
cio_list_node_t* cio_list_pop_back(cio_list_t* handler);

/**
 * @brief Get the first node.
 * @param[in] handler   Pointer to list
 * @return              The first node
 */
cio_list_node_t* cio_list_begin(const cio_list_t* handler);

/**
 * @brief Get the last node.
 * @param[in] handler   The handler of list
 * @return              The last node
 */
cio_list_node_t* cio_list_end(const cio_list_t* handler);

/**
 * @brief Get next node.
 * @param[in] node   Current node
 * @return           The next node
 */
cio_list_node_t* cio_list_next(const cio_list_node_t* node);

/**
 * @brief Get previous node.
 * @param[in] node  current node
 * @return          previous node
 */
cio_list_node_t* cio_list_prev(const cio_list_node_t* node);

/**
 * @brief Move all elements from \p src into the end of \p dst.
 * @param[in] dst   Destination list.
 * @param[in] src   Source list.
 */
void cio_list_migrate(cio_list_t* dst, cio_list_t* src);

/**
 * @}
 */

/**
 * @defgroup CIO_MISC Miscellaneous utilities
 * @{
 */

/**
 * @brief Calculate array size in compile time.
 * @param[in] a Array
 * @return The number of elements in array.
 */
#define CIO_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/**
 * @brief Infinite timeout value.
 */
#define CIO_TIMEOUT_INFINITE ((uint32_t)-1)

/**
 * @brief Get the minimum of two values.
 */
#define CIO_MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @def CIO_OFFSETOF
 * @brief The offset of \p member in \p type.
 */
#if defined(offsetof)
#define CIO_OFFSETOF(type, member) offsetof(type, member)
#else
#define CIO_OFFSETOF(type, member) ((size_t)&(((type*)0)->member))
#endif

/**
 * @def EV_CONTAINER_OF
 * @brief cast a member of a structure out to the containing structure.
 */
#if defined(__GNUC__) || defined(__clang__)
#define CIO_CONTAINER_OF(ptr, type, member)                                    \
    ({                                                                         \
        const typeof(((type*)0)->member)* __mptr = (ptr);                      \
        (type*)((char*)__mptr - CIO_OFFSETOF(type, member));                   \
    })
#else
#define CIO_CONTAINER_OF(ptr, type, member)                                    \
    ((type*)((char*)(ptr) - CIO_OFFSETOF(type, member)))
#endif

/**
 * @brief Get high-resolution time in nanoseconds.
 * @return High-resolution time in nanoseconds.
 */
uint64_t cio_hrtime(void);

/**
 * @brief Initialize global resources.
 * @note It is not necessary to call this function. All functions that relay on
 * global resources will automatically call this function when needed.
 * @note It is safe to call this function multiple times.
 */
void cio_initialize(void);

/**
 * @brief Release all internal library resources.
 * @warning Only call this function once.
 * @warning Don't call this function when there are still event loops or I/O
 *   requests active.
 * @warning Don't call any functions from this library after calling this
 * function.
 */
void cio_shutdown_library(void);

/**
 * @} // GROUP: CIO_MISC
 */

#ifdef __cplusplus
}
#endif

#endif
