#include <io.h>
#include <sys/utime.h>
#define __UTIME_INCLUDED 1
#define ftruncate(h,s) chsize(h,s)
#include "libbeye/sysdep/generic/posix/fileio.c"
