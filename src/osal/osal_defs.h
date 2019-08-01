/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#ifndef _osal_defs_
#define _osal_defs_

#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

// define if debug printf is needed
//#define EC_DEBUG

void debug_print(const char* format, ...);

#ifdef EC_DEBUG
#define EC_PRINT debug_print
#else
#define EC_PRINT(...) do {} while (0)
#endif

#ifndef PACKED
#define PACKED_BEGIN
#define PACKED  __attribute__((__packed__))
#define PACKED_END
#endif

#define OSAL_THREAD_HANDLE HANDLE
#define OSAL_THREAD_FUNC void
#define OSAL_THREAD_FUNC_RT void

#ifdef __cplusplus
}
#endif

#endif
