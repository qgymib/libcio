#ifndef CIO_H
#define CIO_H

/**
 * @brief Infinite timeout value.
 */
#define CIO_TIMEOUT_INFINITE ((uint32_t)-1)

/**
 * @brief Get the minimum of two values.
 */
#define CIO_MIN(a, b) ((a) < (b) ? (a) : (b))

#include "cio/allocator.h"
#include "cio/error.h"
#include "cio/handle.h"
#include "cio/loop.h"
#include "cio/misc.h"
#include "cio/thread.h"

#endif
