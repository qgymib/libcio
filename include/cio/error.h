#ifndef CIO_ERROR_H
#define CIO_ERROR_H

#include <errno.h>

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

#ifdef __cplusplus
extern "C" {
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
    CIO_EUNKNOWN = -4094,
} cio_errno_t;

#ifdef __cplusplus
}
#endif
#endif // CIO_ERROR_H
