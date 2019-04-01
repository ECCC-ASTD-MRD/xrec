#include <sys/resource.h>
#include <stdio.h>
#include <rpnmacros.h>

int f77name(setstacksize) (int *stacksizeinmb)
{
    const rlim_t kStackSize = *stacksizeinmb * 1024 * 1024;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
}

