#include "cio.h"

typedef struct cio_err_str
{
    int         errcode; /* Error code. */
    const char* message; /* Error message. */
} cio_err_str_t;

static cio_err_str_t s_err_msgs[] = {
    { CIO_SUCCESS,         "No error"                                },
    { CIO_EPERM,           "Operation not permitted"                 },
    { CIO_ENOENT,          "No such file or directory"               },
    { CIO_EIO,             "Input/output error"                      },
    { CIO_E2BIG,           "Argument list too long"                  },
    { CIO_EBADF,           "Bad file descriptor"                     },
    { CIO_EAGAIN,          "Resource temporarily unavailable"        },
    { CIO_ENOMEM,          "Not enough space/cannot allocate memory" },
    { CIO_EACCES,          "Permission denied"                       },
    { CIO_EFAULT,          "Bad address"                             },
    { CIO_EBUSY,           "Device or resource busy"                 },
    { CIO_EEXIST,          "File exists"                             },
    { CIO_EXDEV,           "Invalid cross-device link"               },
    { CIO_ENOTDIR,         "Not a directory"                         },
    { CIO_EISDIR,          "Is a directory"                          },
    { CIO_EINVAL,          "Invalid argument"                        },
    { CIO_ENFILE,          "Too many open files in system"           },
    { CIO_EMFILE,          "Too many open files"                     },
    { CIO_ENOSPC,          "No space left on device"                 },
    { CIO_EROFS,           "Read-only filesystem"                    },
    { CIO_EPIPE,           "Broken pipe"                             },
    { CIO_ENAMETOOLONG,    "Filename too long"                       },
    { CIO_ENOSYS,          "Function not implemented"                },
    { CIO_ENOTEMPTY,       "Directory not empty"                     },
    { CIO_ELOOP,           "Too many levels of symbolic links"       },
    { CIO_EPROTO,          "Protocol error"                          },
    { CIO_ENOTSOCK,        "Not a socket"                            },
    { CIO_EMSGSIZE,        "Message too long"                        },
    { CIO_EPROTONOSUPPORT, "Protocol not supported"                  },
    { CIO_ENOTSUP,         "Operation not supported"                 },
    { CIO_EAFNOSUPPORT,    "Address family not supported"            },
    { CIO_EADDRINUSE,      "Address already in use"                  },
    { CIO_EADDRNOTAVAIL,   "Address not available"                   },
    { CIO_ENETUNREACH,     "Network unreachable"                     },
    { CIO_ECONNABORTED,    "Connection aborted"                      },
    { CIO_ECONNRESET,      "Connection reset"                        },
    { CIO_ENOBUFS,         "No buffer space available"               },
    { CIO_EISCONN,         "Socket is connected"                     },
    { CIO_ENOTCONN,        "The socket is not connected"             },
    { CIO_ETIMEDOUT,       "Connection timed out"                    },
    { CIO_ECONNREFUSED,    "Connection refused"                      },
    { CIO_EHOSTUNREACH,    "Host is unreachable"                     },
    { CIO_EALREADY,        "Connection already in progress"          },
    { CIO_EINPROGRESS,     "Operation in progress"                   },
    { CIO_ECANCELED,       "Operation canceled"                      },
    { CIO_EOF,             "End of file"                             },
    { CIO_UNKNOWN,         "Unknown error"                           },
};

const char* cio_strerror(int errcode)
{
    size_t i;
    for (i = 0; i < CIO_ARRAY_SIZE(s_err_msgs); i++)
    {
        if (errcode == s_err_msgs[i].errcode)
        {
            return s_err_msgs[i].message;
        }
    }
    return NULL;
}
