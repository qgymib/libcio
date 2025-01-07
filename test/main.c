#include <cio.h>
#include <stdlib.h>
#include "cutest.h"

int main(int argc, char** argv)
{
    atexit(cio_shutdown_library);
    return cutest_run_tests(argc, argv, stdout, NULL);
}
