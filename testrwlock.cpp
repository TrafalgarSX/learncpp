#include <thread>
#include <iostream>
#include <ReadWriteLock.h>
// #include <ReadWriteLockCpp.h>
#include <chrono>
#include <vector>
#include <condition_variable>
#include <semaphore>

struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        float ms = duration.count() * 1000.0f;
        std::cout << "Timer took " << ms << " ms\n";
    }
};


class ReadWriteLockCpp
{
public:
    ReadWriteLockCpp() : m_readCount(0), m_writeFlag(false), m_writeRequestCount(0), m_readRequestCount(0) {}

    void read_lock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_writeFlag || m_writeRequestCount > 0)
        {
            ++m_readRequestCount;
            m_readCondition.wait(lock, [this](){ return !m_writeFlag && m_writeRequestCount == 0; });
            --m_readRequestCount;
        }

        ++m_readCount;
    }

    void read_unlock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        --m_readCount;
#if 1
        if (m_readCount == 0)
        {
            m_writeCondition.notify_one();
        }
#else 
        if(m_writeRequestCount > 0)
        {
            m_writeCondition.notify_one();
        }
#endif
    }

    void write_lock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if( m_writeFlag || 0 < m_readCount)
        {
            ++m_writeRequestCount;
            m_writeCondition.wait(lock, [this](){ return m_readCount == 0 && !m_writeFlag; });
            --m_writeRequestCount;
        }

        m_writeFlag = true;
    }

    void write_unlock()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_writeFlag = false;
        if(m_writeRequestCount > 0)
        {
            m_writeCondition.notify_one();
        }
        else if(m_readRequestCount > 0)
        {
            m_readCondition.notify_all();
        }
    }


private:
    std::mutex m_mutex;
    std::condition_variable m_readCondition;
    std::condition_variable m_writeCondition;
    int m_readCount;
    bool m_writeFlag;
    int m_writeRequestCount;
    int m_readRequestCount;
};
// https://www.cnblogs.com/fortunely/p/15778050.html

class ReadWriteLockgcc{
public:
    ReadWriteLockgcc() : m_state(0) {}
    ~ReadWriteLockgcc() = default;

    ReadWriteLockgcc(const ReadWriteLockgcc&) = delete;
    ReadWriteLockgcc& operator=(const ReadWriteLockgcc&) = delete;

    void write_lock(){
        std::unique_lock<std::mutex> lock(m_mutex);
        m_gate1.wait(lock, [=](){ return !_M_write_entered(); });
        m_state |= _s_write_entered;
        m_gate2.wait(lock, [=](){ return _M_readers() == 0; });
    }

    void write_unlock(){
        std::unique_lock<std::mutex> lock(m_mutex);
        m_state = 0;
        m_gate1.notify_all();
    }

    void read_lock(){
        std::unique_lock<std::mutex> lock(m_mutex);
        m_gate1.wait(lock, [=](){return m_state < _s_max_readers; });
        ++m_state;
    }

    void read_unlock(){
        std::unique_lock<std::mutex> lock(m_mutex);
        auto prev_state = m_state--;
        if(_M_write_entered())
        {
            if(_M_readers() == 0)
            {
                m_gate2.notify_one();
            }
        }
        else
        {
            if(prev_state == _s_max_readers)
            {
                m_gate1.notify_one();
            }
        }
    }
private:
    std::mutex m_mutex;
    std::condition_variable m_gate1;
    std::condition_variable m_gate2;
    uint32_t m_state;
    static constexpr uint32_t _s_write_entered = 1U << (sizeof(uint32_t) * 8 - 1);
    static constexpr uint32_t _s_max_readers = ~_s_write_entered;

    bool _M_write_entered() const noexcept
    {
        return m_state & _s_write_entered;
    }

    uint32_t _M_readers() const noexcept
    {
        return m_state & _s_max_readers;
    }
};

class shared_mutex
{
    std::mutex mut_;
    std::condition_variable gate1_;
    std::condition_variable gate2_;
    unsigned state_;

    static const unsigned write_entered_ = 1U << (sizeof(unsigned)*CHAR_BIT - 1);
    static const unsigned n_readers_ = ~write_entered_;

public:

    shared_mutex() : state_(0) {}

// Exclusive ownership

    void write_lock();
    void write_unlock();

// Shared ownership

    void read_lock();
    void read_unlock();
};

// Exclusive ownership

void
shared_mutex::write_lock()
{
    // std::this_thread::disable_interruption _;
    std::unique_lock<std::mutex> lk(mut_);
    while (state_ & write_entered_)
        gate1_.wait(lk);
    state_ |= write_entered_;
    while (state_ & n_readers_)
        gate2_.wait(lk);
}

void
shared_mutex::write_unlock()
{
    {
    std::unique_lock<std::mutex> _(mut_);
    state_ = 0;
    }
    gate1_.notify_all();
}

// Shared ownership

void
shared_mutex::read_lock()
{
    std::unique_lock<std::mutex> lk(mut_);
    while ((state_ & write_entered_) || (state_ & n_readers_) == n_readers_)
        gate1_.wait(lk);
    unsigned num_readers = (state_ & n_readers_) + 1;
    state_ &= ~n_readers_;
    state_ |= num_readers;
}

void
shared_mutex::read_unlock()
{
    std::unique_lock<std::mutex> _(mut_);
    unsigned num_readers = (state_ & n_readers_) - 1;
    state_ &= ~n_readers_;
    state_ |= num_readers;
    if (state_ & write_entered_)
    {
        if (num_readers == 0)
            gate2_.notify_one();
    }
    else
    {
        if (num_readers == n_readers_ - 1)
            gate1_.notify_one();
    }
}


// ReadWriteLock rwlock;
// ReadWriteLockCpp rwlock;
// ReadWriteLockgcc rwlock;
shared_mutex rwlock;
int shared_data = 0;

#include <shared_mutex>
std::shared_timed_mutex smutex;
// #define USE_STD

void reader() {
    for (int i = 0; i < 100; ++i) {
#ifdef USE_STD
        std::shared_lock<std::shared_timed_mutex> lock(smutex);
#else
        rwlock.read_lock();
#endif
        std::cout << "Read: " << shared_data << "\n";
#ifdef USE_STD
       lock.unlock();
#else
        rwlock.read_unlock();
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void writer() {
    for (int i = 0; i < 100; ++i) {
#ifdef USE_STD
        std::unique_lock<std::shared_timed_mutex> lock(smutex);
#else
        rwlock.write_lock();
#endif
        // shared_data = i;
        ++shared_data;
        std::cout << "Write: " << shared_data << "\n";
#ifdef USE_STD
        lock.unlock();
#else
        rwlock.write_unlock();
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    int num_readers = 50;
    int num_writers = 3;

    Timer timer;

    std::vector<std::thread> reader_threads;
    std::vector<std::thread> writer_threads;

    for (int i = 0; i < num_readers; ++i) {
        reader_threads.push_back(std::thread(reader));
    }

    for (int i = 0; i < num_writers; ++i) {
        writer_threads.push_back(std::thread(writer));
    }

    for (int i = 0; i < num_readers; ++i) {
        reader_threads[i].join();
    }

    for (int i = 0; i < num_writers; ++i) {
        writer_threads[i].join();
    }

    std::cout << "Final value: " << shared_data << std::endl;
    return 0;
}