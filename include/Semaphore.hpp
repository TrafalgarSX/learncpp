#pragma once
#include <cassert>

#if defined(_WIN32)
//---------------------------------------------------------
// Semaphore (Windows)
//---------------------------------------------------------

#include <windows.h>
#undef min
#undef max

class Semaphore
{
private:
    HANDLE m_hSema;

    Semaphore(const Semaphore& other) = delete;
    Semaphore& operator=(const Semaphore& other) = delete;

public:
    Semaphore(int initialCount = 0)
    {
        assert(initialCount >= 0);
        m_hSema = CreateSemaphore(NULL, initialCount, MAXLONG, NULL);
    }

    ~Semaphore()
    {
        CloseHandle(m_hSema);
    }

    void acquire()
    {
        WaitForSingleObject(m_hSema, INFINITE);
    }

    void release(int count = 1)
    {
        ReleaseSemaphore(m_hSema, count, NULL);
    }
};

#elif defined(__unix__)

//---------------------------------------------------------
// Semaphore (POSIX, Linux)
//---------------------------------------------------------

#include <semaphore.h>

class Semaphore
{
private:
    sem_t m_sema;

    Semaphore(const Semaphore& other) = delete;
    Semaphore& operator=(const Semaphore& other) = delete;

public:
    Semaphore(int initialCount = 0)
    {
        assert(initialCount >= 0);
        sem_init(&m_sema, 0, initialCount);
    }

    ~Semaphore()
    {
        sem_destroy(&m_sema);
    }

    void acquire()
    {
        // http://stackoverflow.com/questions/2013181/gdb-causes-sem-wait-to-fail-with-eintr-error
#if 0
        int rc;
        do
        {
            rc = sem_wait(&m_sema);
        }
        while (rc == -1 && errno == EINTR);
#else
        for (;;)
        {
          auto __err = sem_wait(&m_sema);
          if (__err && (errno == EINTR))
            continue;
          else if (__err)
            std::terminate();
          else
            break;
        }
#endif
    }

    void release()
    {
        sem_post(&m_sema);
    }

    void release(int count)
    {
#if 0
        while (count-- > 0)
        {
            sem_post(&m_sema);
        }
#else
      for(; count != 0; --count)
      {
        auto __err = sem_post(&m_sema);
        if (__err)
          std::terminate();
      }
#endif
    }
};

#endif
