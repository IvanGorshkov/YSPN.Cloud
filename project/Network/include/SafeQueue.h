#ifndef _SAFEQUEUE_H
#define _SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class SafeQueue {
public:
    SafeQueue();

    ~SafeQueue();

    // Add an element to the queue.
    void Push(T t);

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T Pop();

    bool Empty();

private:
    std::queue<T> _q;

    mutable std::mutex _m;

    std::condition_variable _c;
};



template<class T>
SafeQueue<T>::SafeQueue() {}

template<class T>
SafeQueue<T>::~SafeQueue() = default;

template<class T>
void SafeQueue<T>::Push(T t) {
    std::lock_guard<std::mutex> lock(_m);
    _q.push(t);
    _c.notify_one();
}

template<class T>
T SafeQueue<T>::Pop() {
    std::unique_lock<std::mutex> lock(_m);
    while (_q.empty()) {
        // release lock as long as the wait and require it afterwards.
        _c.wait(lock);
    }
    T val = _q.front();
    _q.pop();
    return val;
}

template<class T>
bool SafeQueue<T>::Empty() {
    return _q.empty();
}

#endif // _SAFEQUEUE_H
