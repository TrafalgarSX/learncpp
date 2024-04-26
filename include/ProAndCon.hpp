#pragma once

#include <iostream>
#include <list>
#include <deque>
#include <mutex>
#include <condition_variable>

template<typename T>
class ProAndCon {
private:
    // std::list<T> m_queue;
    std::deque<T> m_queue;
    std::mutex m_mutex;//全局互斥锁
    std::condition_variable_any m_notEmpty;//全局条件变量（不为空）
    std::condition_variable_any m_notFull;//全局条件变量（不为满）
    int m_maxSize;//队列最大容量

private:
    //队列为空
    bool isEmpty() const {
        return m_queue.empty();
    }
    //队列已满
    bool isFull() const {
        return m_queue.size() == m_maxSize;
    }

public:
    ProAndCon(int maxSize): m_maxSize(maxSize), m_queue(maxSize) {
    }
    ProAndCon();

    virtual ~ProAndCon();

    void product(const T& v) {
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_notFull.wait(locker, [=]{return isFull();});
            //往队列里面生产一个元素,同时通知不为空这个信号量
            m_queue.push_back(v);
        }
        m_notEmpty.notify_one();
    }

    void consumption(T& v) {
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_notEmpty.wait(locker, [=]{return isEmpty();});
            //在队列里面消费一个元素,同时通知队列不满这个信号量
            v = m_queue.front();
            m_queue.pop_front();
        }
        m_notFull.notify_one();
    }
};
