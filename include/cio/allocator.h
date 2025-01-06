#ifndef CIO_ALLOCATOR_H
#define CIO_ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Changes the size of the memory block pointed to by \p ptr to \p size
 *   bytes.
 * @see https://man7.org/linux/man-pages/man3/realloc.3p.html
 */
typedef void* (*cio_allocator_fn)(void* ptr, size_t size);

/**
 * @brief Replace allocator with new function.
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

#ifdef __cplusplus
}
#endif
#endif // CIO_ALLOCATOR_H
