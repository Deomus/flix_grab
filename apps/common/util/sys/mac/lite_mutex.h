//Synchro.h

#ifndef COMMON_MAC__SYNCHRO_H
#define COMMON_MAC__SYNCHRO_H

#include <mutex>

namespace sys
{

class Mutex : ILockable
{
public:
    Mutex()
    {
        //::InitializeCriticalSection(&m_mutex);
    }
    virtual ~Mutex()
    {
        //::DeleteCriticalSection(&m_mutex);
    }
    virtual void lock()
    {
        m_mutex.lock();
    }
    virtual bool try_lock()
    {
        return m_mutex.try_lock();
    }
    virtual void unlock()
    {
        m_mutex.unlock();
    }
private:
    Mutex(const Mutex &other){}
    Mutex& operator=(const Mutex &other) { return *this; }
private:
    mutable std::mutex m_mutex;
};

template <typename T>
class LockableObj : public ILockable
{
public:
    LockableObj()
    {
    }
    virtual ~LockableObj()
    {
    }
    virtual void lock()
    {
        m_mutex.lock();
    }
    virtual bool try_lock()
    {
        return m_mutex.try_lock();
    }
    virtual void unlock()
    {
        m_mutex.lock();
    }
private:
    LockableObj(LockableObj &other);
    LockableObj& operator=(const LockableObj &other);
public:
private:
    mutable std::mutex m_mutex;
public:
    T o;
};

class ScopedLock
{
private:
    ScopedLock(ScopedLock &) {}
public:
    ScopedLock(ILockable *pLockObj) :
        m_pLock(pLockObj),
        m_bLocked(false)
    {
        lock();
    }    
    ~ScopedLock()
    {
        unlock();
    }
    void lock()
    {
        m_bLocked = m_pLock->try_lock();
        if (!m_bLocked)
            m_pLock->lock();
    }
    bool try_lock()
    {
        m_bLocked = m_pLock->try_lock();
        return m_bLocked;
    }
    void unlock()
    {
        m_pLock->unlock();
    }
private:
    ILockable           *m_pLock;
    volatile bool       m_bLocked;
};


}

#endif //COMMON_MAC__SYNCHRO_H
