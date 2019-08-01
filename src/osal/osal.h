/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#ifndef _osal_
#define _osal_

#ifdef __cplusplus
extern "C"
{
#endif

#include "osal_defs.h"
#include <stdint.h>
#include <stdbool.h>

/* General types */

#ifndef Arduino_h
typedef bool                boolean;
#endif
#define TRUE                1
#define FALSE               0
typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef uint8_t             uint8;
typedef uint16_t            uint16;
typedef uint32_t            uint32;
typedef int64_t             int64;
typedef uint64_t            uint64;
typedef float               float32;
typedef double              float64;

// TODO ec_timet is not compatible with regular SOEM.
#if 0
typedef struct
{
    uint32 sec;     /*< Seconds elapsed since the Epoch (Jan 1, 1970) */
    uint32 usec;    /*< Microseconds elapsed since last second boundary */
} ec_timet;

typedef struct osal_timer
{
    ec_timet stop_time;
} osal_timert;

#else

// elapsed time [usec] from start-up, not linux time
typedef uint32 ec_timet;

typedef struct osal_timer
{
    ec_timet start_time;
    ec_timet timeout;
} osal_timert;

#endif

void osal_timer_start(osal_timert * self, uint32 timeout_us);
boolean osal_timer_is_expired(osal_timert * self);
int osal_usleep(uint32 usec);
ec_timet osal_current_time(void);
void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff);
int osal_thread_create(void *thandle, int stacksize, void *func, void *param);
int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param);

#ifdef __cplusplus
}
#endif

#endif
