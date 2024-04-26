/*
本文件内是实现读写锁功能，不同操作系统有不同的实现方法
在WINDOWS平台下，是通过调用如下几个函数实现：
	初始化读写锁：	InitializeSRWLock
	释放写锁：		ReleaseSRWLockExclusive
	释放读锁：		ReleaseSRWLockShared
	申请写锁：		AcquireSRWLockExclusive
	申请读锁：		AcquireSRWLockShared
	尝试申请：		TryAcquireSRWLockExclusive  -- WIN7以上才有
	尝试申请：		TryAcquireSRWLockShared		-- WIN7以上才有
在LINUX平台下，是通过调用如下几个函数实现：
	初始化读写锁：	int pthread_rwlock_init(pthread_rwlock_t* __rwlock, const pthread_rwlockattr_t* __attr);	// 第二个参数是属性，可以设置为NULL
	销毁读写锁：	int pthread_rwlock_destroy(pthread_rwlock_t* __rwlock);
	申请读锁：		int pthread_rwlock_rdlock(pthread_rwlock_t* __rwlock);
	解锁：			int pthread_rwlock_unlock(pthread_rwlock_t* __rwlock);
	申请写锁：		int pthread_rwlock_wrlock(pthread_rwlock_t* __rwlock);
	尝试申请：		int pthread_rwlock_tryrdlock(pthread_rwlock_t* __rwlock);
	尝试申请：		int pthread_rwlock_trywrlock(pthread_rwlock_t* __rwlock);
本文件是对读写锁做一次封装，使得上层调用保持一致
*/

#include <ReadWriteLockc.h>
#ifdef __cplusplus
extern "C"
{
#endif

#if defined (WIN32)
#  define _OS_WINDOWS_
#endif

#if defined(_OS_WINDOWS_)
#	include <Windows.h>
	typedef SRWLOCK				ETRWLOCK;
#else
#    include <pthread.h>
	typedef pthread_rwlock_t	ETRWLOCK;
#endif

uint32_t EtRwLockCreate(ETHANDLE* phHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)malloc(sizeof(ETRWLOCK));
	if ( NULL == pRwLock ){
		return ERROR_NOT_ENOUGH_MEMORY;
	}
#if defined(_OS_WINDOWS_)
	InitializeSRWLock(pRwLock);
	u32Result = 0x00;
#else
	u32Result = pthread_rwlock_init(pRwLock, NULL);
	if ( 0x00 != u32Result ){
		free(pRwLock);
		return u32Result;
	}
#endif
	*phHandle = (ETHANDLE)pRwLock;
	return 0x00;
}

void  EtRwLockDestroy(ETHANDLE* phHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	if ( NULL == phHandle || NULL == *phHandle ){
		return;
	}
	pRwLock = (ETRWLOCK*)(*phHandle);
#if defined(_OS_WINDOWS_)
	u32Result = 0x00;
#else
	u32Result = pthread_rwlock_destroy(pRwLock);
#endif
	free(pRwLock);
	*phHandle = NULL;
	return;
}

uint32_t EtRwLockAcquireRead(ETHANDLE hHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)hHandle;
#if defined(_OS_WINDOWS_)
	AcquireSRWLockShared(pRwLock);
	u32Result = 0x00;
#else
	u32Result = pthread_rwlock_rdlock(pRwLock);
#endif
	return u32Result;
}

uint32_t EtRwLockAcquireWrite(ETHANDLE hHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)hHandle;
#if defined(_OS_WINDOWS_)
	AcquireSRWLockExclusive(pRwLock);
	u32Result = 0x00;
#else
	u32Result = pthread_rwlock_wrlock(pRwLock);
#endif
	return u32Result;
}

uint32_t EtRwLockTryWrite(ETHANDLE hHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)hHandle;
#if defined(_OS_WINDOWS_)
#	if _MSC_VER > 1500
	u32Result = TryAcquireSRWLockExclusive(pRwLock);	// 只有WIN7以上才支持
#	else
	AcquireSRWLockExclusive(pRwLock);					// VS2008不支持TRY功能
	u32Result = 0x00;
#	endif
#else
	u32Result = pthread_rwlock_trywrlock(pRwLock);
#endif
	return u32Result;
}

uint32_t EtRwLockTryRead(ETHANDLE hHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)hHandle;
#if defined(_OS_WINDOWS_)
#	if _MSC_VER > 1500
	u32Result = TryAcquireSRWLockShared(pRwLock);	// 只有WIN7以上才支持
#	else
	AcquireSRWLockShared(pRwLock);					// VS2008不支持TRY功能
	u32Result = 0x00;
#	endif
#else
	u32Result = pthread_rwlock_tryrdlock(pRwLock);
#endif
	return u32Result;
}

uint32_t EtRwLockReleaseRead(ETHANDLE hHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)hHandle;
#if defined(_OS_WINDOWS_)
	ReleaseSRWLockShared(pRwLock);
	u32Result = 0x00;
#else
	u32Result = pthread_rwlock_unlock(pRwLock);
#endif
	return u32Result;
}

uint32_t EtRwLockReleaseWrite(ETHANDLE hHandle)
{
	uint32_t					u32Result;
	ETRWLOCK*			pRwLock;

	pRwLock = (ETRWLOCK*)hHandle;
#if defined(_OS_WINDOWS_)
	ReleaseSRWLockExclusive(pRwLock);
	u32Result = 0x00;
#else
	u32Result = pthread_rwlock_unlock(pRwLock);
#endif
	return u32Result;
}

#ifdef __cplusplus
};
#endif
