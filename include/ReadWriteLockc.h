#ifndef __READ_WRITE_LOCK_C_H_
#define __READ_WRITE_LOCK_C_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef void* ETHANDLE;
#ifdef __cplusplus
extern "C"
{
#endif
uint32_t EtRwLockCreate(ETHANDLE* phHandle);
void  EtRwLockDestroy(ETHANDLE* phHandle);
uint32_t EtRwLockAcquireRead(ETHANDLE hHandle);
uint32_t EtRwLockAcquireWrite(ETHANDLE hHandle);
uint32_t EtRwLockTryWrite(ETHANDLE hHandle);
uint32_t EtRwLockTryRead(ETHANDLE hHandle);
uint32_t EtRwLockReleaseRead(ETHANDLE hHandle);
uint32_t EtRwLockReleaseWrite(ETHANDLE hHandle);

#ifdef __cplusplus
}
#endif

#endif