#ifndef CIO_LOOP_H
#define CIO_LOOP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
#endif // CIO_LOOP_H
