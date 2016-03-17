#include "Mutex.hpp"

const pthread_mutex_t mutex::recmutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
const pthread_cond_t condition_var::cond  = PTHREAD_COND_INITIALIZER;

mutex::mutex( )
{
    m_mutex_lock = recmutex;
}

void
mutex::lock( )
{
    pthread_mutex_lock( &m_mutex_lock );
}


void
mutex::unlock( )
{
    pthread_mutex_unlock( &m_mutex_lock );
}

condition_var::condition_var( )
{
    m_cond = cond;
}


void
condition_var::signal( )
{
    pthread_cond_signal( &m_cond );
}

void
condition_var::wait( )
{
    pthread_cond_wait( &m_cond, &m_mutex_lock );
}


