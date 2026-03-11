
#ifndef INTERLOCKED_H
#define INTERLOCKED_H

#include "base_type_define.h"

class Interlocked
{
public:
    Interlocked(tint32 defval=0);
    ~Interlocked();

    tint32 Decrement();//减1
    tint32 Increment();//加1

    ///返回老的值
    tint32 SetValue(tint32 val);

    ////////////返回被加变量的初始值
    tint32 ExchangeAdd(tint32 val);

	operator tint32() const;
private:

    volatile tint32 m_counter;//////计数
};

#endif // MY_THREAD_H
