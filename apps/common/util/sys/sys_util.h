//sys_util.h

#ifndef COMMON__SYS_UTIL_H
#define COMMON__SYS_UTIL_H

#ifndef UNUZD
#define UNUZD(a) (void)a
#endif

#include "win32/sys_util.h"


namespace sys
{

class RetCode
{
public:
    unsigned int    c1;
    int             c2;

    RetCode()
        : c1(0)
        , c2(0)
    {
    }

    RetCode(unsigned int nCode1, int nCode2)
        : c1(nCode1)
        , c2(nCode2)
    {
        c1 = nCode1;
        c2 = nCode2;
    }

    void set(unsigned int nCode1, int nCode2)
    {
        c1 = nCode1;
        c2 = nCode2;
    }

    bool failed()
    {
        return c1 != 0 || c2 != 0;
    }

    bool succeeded()
    {
        return !this->failed();
    }

    std::string m_sMsg;
};

class ShortDate
{
private:
    int m_nYear;
    int m_nMonth;
    int m_nDay;

public:
    ShortDate()
        : m_nYear(0)
        , m_nMonth(0)
        , m_nDay(0)
    {
    }
    int year() const
    {
        return m_nYear;
    }
    void setYear(int val)
    {
        if (val < 0)
            val = 0;

        m_nYear = val;
    }
    int month() const
    {
        return m_nMonth;
    }
    void setMonth(int val)
    {
        if (   val < 0
            || val > 12)
        {
            val = 0;
        }

        m_nMonth = val;
    }
    int day() const
    {
        return m_nDay;
    }
    void setDay(int val)
    {
        if (   val < 0
            || val > 31)
        {
            val = 0;
        }

        m_nDay = val;
    }
};

inline int dateToDaysFrom1960(const sys::ShortDate& dmy)
{
    int nRes = 0;
    nRes = (dmy.year() - 1960) * 364 + (dmy.year() - 1960) / 4;
    nRes += 364;
    switch (dmy.month())
    {
        case 1:nRes -= 31; 
        case 2: nRes -= 28; 
        case 3: nRes -= 31;
        case 4: nRes -= 30;
        case 5: nRes -= 31;
        case 6: nRes -= 30;
        case 7: nRes -= 31;
        case 8: nRes -= 30;
        case 9: nRes -= 31;
        case 10: nRes -= 30;
        case 11: nRes -= 31;
        case 12: nRes -= 30;
    }

    if (dmy.year() % 1960 != 0)
        nRes++;

    nRes += dmy.day();
    return nRes;
}

}

#endif //COMMON__SYS_UTIL_H
