#ifndef _SYSINFO_H_
#define _SYSINFO_H_

#include "types.h"

struct sysinfo {
  uint64 freemem;   // số byte bộ nhớ trống
  uint64 nproc;     // số tiến trình (state != UNUSED)
};

#endif // _SYSINFO_H_
