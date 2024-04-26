#include <ReadWriteLockc.h>
#include <stdexcept>

class ReadWriteLock {
public:
    ReadWriteLock() {
        uint32_t result = EtRwLockCreate(&rwlock);
        if (result != 0) {
            throw std::runtime_error("Failed to create read-write lock");
        }
    }

    ~ReadWriteLock() {
        EtRwLockDestroy(&rwlock);
    }

    ReadWriteLock(const ReadWriteLock&) = delete;
    ReadWriteLock& operator=(const ReadWriteLock&) = delete;

    uint32_t read_lock() {
        return EtRwLockAcquireRead(rwlock);
    }

    uint32_t read_unlock() {
        return EtRwLockReleaseRead(rwlock);
    }

    uint32_t write_lock() {
        return EtRwLockAcquireWrite(rwlock);
    }

    uint32_t write_unlock() {
        return EtRwLockReleaseWrite(rwlock);
    }

private:
    ETHANDLE rwlock;
};