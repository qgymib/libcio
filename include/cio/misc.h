#ifndef CIO_MISC_H
#define CIO_MISC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup CIO_MISC Miscellaneous utilities
 * @{
 */

/**
 * @brief Get high-resolution time in nanoseconds.
 * @return High-resolution time in nanoseconds.
 */
uint64_t cio_hrtime(void);

/**
 * @brief Release all internal library resources.
 * @warning Only call this function once.
 * @warning Don't call this function when there are still event loops or I/O
 *   requests active.
 * @warning Don't call any functions from this library after calling this function.
 */
void cio_shutdown_library(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif // CIO_MISC_H
