#ifndef CIO_ERROR_WIN_H
#define CIO_ERROR_WIN_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Translate system error into cio error.
 * @param[in] sys_errno System error
 * @return CIO error.
 */
int cio_translate_sys_error(int sys_errno);

#ifdef __cplusplus
}
#endif
#endif
