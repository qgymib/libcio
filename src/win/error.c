#include "cio.h"
#include "error.h"

typedef struct err_pair
{
    int sys;  /* System error */
    int wrap; /* CIO error */
} err_pair_t;

static const err_pair_t errors[] = {
    { WSAEACCES,                        CIO_EACCES          },
    { ERROR_ELEVATION_REQUIRED,         CIO_EACCES          },
    { ERROR_CANT_ACCESS_FILE,           CIO_EACCES          },
    { ERROR_ADDRESS_ALREADY_ASSOCIATED, CIO_EADDRINUSE      },
    { WSAEADDRINUSE,                    CIO_EADDRINUSE      },
    { WSAEADDRNOTAVAIL,                 CIO_EADDRNOTAVAIL   },
    { WSAEAFNOSUPPORT,                  CIO_EAFNOSUPPORT    },
    { WSAEWOULDBLOCK,                   CIO_EAGAIN          },
    { ERROR_NO_DATA,                    CIO_EAGAIN          },
    { WSAEALREADY,                      CIO_EALREADY        },
    { ERROR_INVALID_FLAGS,              CIO_EBADF           },
    { ERROR_INVALID_HANDLE,             CIO_EBADF           },
    { ERROR_LOCK_VIOLATION,             CIO_EBUSY           },
    { ERROR_PIPE_BUSY,                  CIO_EBUSY           },
    { ERROR_SHARING_VIOLATION,          CIO_EBUSY           },
    { ERROR_OPERATION_ABORTED,          CIO_ECANCELED       },
    { WSAEINTR,                         CIO_ECANCELED       },
    { ERROR_CONNECTION_ABORTED,         CIO_ECONNABORTED    },
    { WSAECONNABORTED,                  CIO_ECONNABORTED    },
    { ERROR_CONNECTION_REFUSED,         CIO_ECONNREFUSED    },
    { WSAECONNREFUSED,                  CIO_ECONNREFUSED    },
    { ERROR_NETNAME_DELETED,            CIO_ECONNRESET      },
    { WSAECONNRESET,                    CIO_ECONNRESET      },
    { ERROR_ALREADY_EXISTS,             CIO_EEXIST          },
    { ERROR_FILE_EXISTS,                CIO_EEXIST          },
    { ERROR_NOACCESS,                   CIO_EFAULT          },
    { WSAEFAULT,                        CIO_EFAULT          },
    { ERROR_HOST_UNREACHABLE,           CIO_EHOSTUNREACH    },
    { WSAEHOSTUNREACH,                  CIO_EHOSTUNREACH    },
    { ERROR_INSUFFICIENT_BUFFER,        CIO_EINVAL          },
    { ERROR_INVALID_DATA,               CIO_EINVAL          },
    { ERROR_INVALID_PARAMETER,          CIO_EINVAL          },
    { ERROR_SYMLINK_NOT_SUPPORTED,      CIO_EINVAL          },
    { WSAEINVAL,                        CIO_EINVAL          },
    { WSAEPFNOSUPPORT,                  CIO_EINVAL          },
    { ERROR_BEGINNING_OF_MEDIA,         CIO_EIO             },
    { ERROR_BUS_RESET,                  CIO_EIO             },
    { ERROR_CRC,                        CIO_EIO             },
    { ERROR_DEVICE_DOOR_OPEN,           CIO_EIO             },
    { ERROR_DEVICE_REQUIRES_CLEANING,   CIO_EIO             },
    { ERROR_DISK_CORRUPT,               CIO_EIO             },
    { ERROR_EOM_OVERFLOW,               CIO_EIO             },
    { ERROR_FILEMARK_DETECTED,          CIO_EIO             },
    { ERROR_GEN_FAILURE,                CIO_EIO             },
    { ERROR_INVALID_BLOCK_LENGTH,       CIO_EIO             },
    { ERROR_IO_DEVICE,                  CIO_EIO             },
    { ERROR_NO_DATA_DETECTED,           CIO_EIO             },
    { ERROR_NO_SIGNAL_SENT,             CIO_EIO             },
    { ERROR_OPEN_FAILED,                CIO_EIO             },
    { ERROR_SETMARK_DETECTED,           CIO_EIO             },
    { ERROR_SIGNAL_REFUSED,             CIO_EIO             },
    { WSAEISCONN,                       CIO_EISCONN         },
    { ERROR_CANT_RESOLVE_FILENAME,      CIO_ELOOP           },
    { ERROR_TOO_MANY_OPEN_FILES,        CIO_EMFILE          },
    { WSAEMFILE,                        CIO_EMFILE          },
    { WSAEMSGSIZE,                      CIO_EMSGSIZE        },
    { ERROR_BUFFER_OVERFLOW,            CIO_ENAMETOOLONG    },
    { ERROR_FILENAME_EXCED_RANGE,       CIO_ENAMETOOLONG    },
    { ERROR_NETWORK_UNREACHABLE,        CIO_ENETUNREACH     },
    { WSAENETUNREACH,                   CIO_ENETUNREACH     },
    { WSAENOBUFS,                       CIO_ENOBUFS         },
    { ERROR_BAD_PATHNAME,               CIO_ENOENT          },
    { ERROR_DIRECTORY,                  CIO_ENOENT          },
    { ERROR_ENVVAR_NOT_FOUND,           CIO_ENOENT          },
    { ERROR_FILE_NOT_FOUND,             CIO_ENOENT          },
    { ERROR_INVALID_NAME,               CIO_ENOENT          },
    { ERROR_INVALID_DRIVE,              CIO_ENOENT          },
    { ERROR_INVALID_REPARSE_DATA,       CIO_ENOENT          },
    { ERROR_MOD_NOT_FOUND,              CIO_ENOENT          },
    { ERROR_PATH_NOT_FOUND,             CIO_ENOENT          },
    { WSAHOST_NOT_FOUND,                CIO_ENOENT          },
    { WSANO_DATA,                       CIO_ENOENT          },
    { ERROR_NOT_ENOUGH_MEMORY,          CIO_ENOMEM          },
    { ERROR_OUTOFMEMORY,                CIO_ENOMEM          },
    { ERROR_CANNOT_MAKE,                CIO_ENOSPC          },
    { ERROR_DISK_FULL,                  CIO_ENOSPC          },
    { ERROR_EA_TABLE_FULL,              CIO_ENOSPC          },
    { ERROR_END_OF_MEDIA,               CIO_ENOSPC          },
    { ERROR_HANDLE_DISK_FULL,           CIO_ENOSPC          },
    { ERROR_NOT_CONNECTED,              CIO_ENOTCONN        },
    { WSAENOTCONN,                      CIO_ENOTCONN        },
    { ERROR_DIR_NOT_EMPTY,              CIO_ENOTEMPTY       },
    { WSAENOTSOCK,                      CIO_ENOTSOCK        },
    { ERROR_NOT_SUPPORTED,              CIO_ENOTSUP         },
    { ERROR_BROKEN_PIPE,                CIO_EOF             },
    { ERROR_ACCESS_DENIED,              CIO_EPERM           },
    { ERROR_PRIVILEGE_NOT_HELD,         CIO_EPERM           },
    { ERROR_BAD_PIPE,                   CIO_EPIPE           },
    { ERROR_PIPE_NOT_CONNECTED,         CIO_EPIPE           },
    { WSAESHUTDOWN,                     CIO_EPIPE           },
    { WSAEPROTONOSUPPORT,               CIO_EPROTONOSUPPORT },
    { ERROR_WRITE_PROTECT,              CIO_EROFS           },
    { ERROR_SEM_TIMEOUT,                CIO_ETIMEDOUT       },
    { WSAETIMEDOUT,                     CIO_ETIMEDOUT       },
    { ERROR_NOT_SAME_DEVICE,            CIO_EXDEV           },
    { ERROR_INVALID_FUNCTION,           CIO_EISDIR          },
};

int cio_translate_sys_error(int sys_errno)
{
    if (sys_errno <= 0)
    {
        return sys_errno;
    }

    size_t i;
    for (i = 0; i < CIO_ARRAY_SIZE(errors); i++)
    {
        const err_pair_t* p = &errors[i];
        if (p->sys == sys_errno)
        {
            return p->wrap;
        }
    }
    return CIO_UNKNOWN;
}
