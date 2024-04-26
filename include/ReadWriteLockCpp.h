#ifndef __READ_WRITE_LOCK_CPP_H_
#define __READ_WRITE_LOCK_CPP_H_

#include <iostream>
#include <mutex>


class ReadWriteLockCpp
{
public:
    ReadWriteLockCpp() : m_readCount(0), m_writeCount(0), m_writeRequestCount(0) {}

    void read_lock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_readCondition.wait(lock, [this](){ return m_writeCount == 0; });
        ++m_readCount;
    }

    void read_unlock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        --m_readCount;
        if (m_readCount == 0)
        {
            m_writeCondition.notify_one();
        }
    }

    void write_lock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        ++m_writeRequestCount;
        m_writeCondition.wait(lock, [this](){ return m_readCount == 0 && m_writeCount == 0; });
        ++m_writeCount;
    }

    void write_unlock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        --m_writeCount;
        if (m_writeCount == 0)
        {
            m_readCondition.notify_all();
        }
        else
        {
            m_writeCondition.notify_one();
        }
    }


private:
    std::mutex m_mutex;
    std::condition_variable m_readCondition;
    std::condition_variable m_writeCondition;
    int m_readCount;
    int m_writeCount;
    int m_writeRequestCount;
};


#endif