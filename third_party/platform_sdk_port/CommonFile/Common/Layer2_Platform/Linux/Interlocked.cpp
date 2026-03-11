
#include "Interlocked.h"

//////////////////////////////////////////////////////////////
Interlocked::Interlocked(tint32 defval/*=0*/) : m_counter(defval)
{
}

//////////////////////////////////////////////////////////////
Interlocked::~Interlocked()
{
}

tint32 Interlocked::Decrement()
{
	return __sync_sub_and_fetch(&m_counter, 1);
}

tint32 Interlocked::Increment()
{
    return __sync_add_and_fetch(&m_counter, 1);
}

tint32 Interlocked::SetValue(tint32 val)
{
	__sync_synchronize();
	return __sync_lock_test_and_set(&m_counter, val);
}

tint32 Interlocked::ExchangeAdd(tint32 val)
{
	return __sync_fetch_and_add(&m_counter, val);
}

Interlocked::operator tint32() const
{
	return m_counter;
}
