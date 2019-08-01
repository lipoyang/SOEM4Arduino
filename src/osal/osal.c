/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#include <Arduino.h>

#if defined(GRROSE)
#include "FreeRTOS.h"
#include "task.h"
#endif

#include "osal.h"

ec_timet osal_current_time (void)
{
   uint32 ret = micros();
   return ret;
}

void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff)
{
   *diff = *end - *start;
}

void osal_timer_start (osal_timert *self, uint32 timeout_usec)
{
    self->start_time = micros();
    self->timeout = timeout_usec;
}

boolean osal_timer_is_expired (osal_timert *self)
{
    uint32 now = micros();
    uint32 elapsed = now - self->start_time;
    
    return (elapsed >= self->timeout);
}

int osal_usleep (uint32 usec)
{
#if defined(GRROSE)
    uint32 msec;
    msec = usec / 1000;
    usec = usec % 1000;
    vTaskDelay(msec);
#else
    delayMicroseconds(usec);
#endif
    return 0;
}

void *osal_malloc(size_t size)
{
   return malloc(size);
}

void osal_free(void *ptr)
{
   free(ptr);
}

int osal_thread_create(void *thandle, int stacksize, void *func, void *param)
{
#if defined(GRROSE)
    // TODO supports multi-task
#else
    // not support multi-task
    return 1;
#endif
}

int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param)
{
#if defined(GRROSE)
    // TODO supports multi-task
#else
    // not support multi-task
    return 1;
#endif
}
