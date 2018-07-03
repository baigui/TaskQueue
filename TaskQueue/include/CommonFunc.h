/**
 */

#ifndef PROJECT_COMMONFUNC_H
#define PROJECT_COMMONFUNC_H

#include <stdint.h>

#define MIN_INT64 (-9223372036854775807LL -1)

#ifdef __cplusplus
extern "C" {
#endif

int64_t getCurrentTimeUsec();

void sleepUs(int64_t us);

void initSleepFunc();

void deinitSleepFunc();

void printStackTrace();

#ifdef __cplusplus
}
#endif

#endif /*PROJECT_COMMONFUNC_H*/
