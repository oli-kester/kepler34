#pragma once

#include "Globals.hpp"

#include <pthread.h>

class Mutex {

private:

    static const pthread_mutex_t recmutex;

protected:

    /* mutex lock */
    pthread_mutex_t  m_mutex_lock;

public:

    Mutex();

    void lock();
    void unlock();

};

class condition_var : public Mutex {

private:

    static const pthread_cond_t cond;

    pthread_cond_t m_cond;

public:

    condition_var();

    void wait();
    void signal();

};

