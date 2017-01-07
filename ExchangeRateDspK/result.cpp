#include "result.h"

Result::Result(){

}

Result::~Result(){

}

QString Result::date() const
{
    return m_date;
}

void Result::setDate(const QString &date)
{
    m_date = date;
}

double Result::open() const
{
    return m_open;
}

void Result::setOpen(double open)
{
    m_open = open;
}

double Result::close() const
{
    return m_close;
}

void Result::setClose(double close)
{
    m_close = close;
}

double Result::high() const
{
    return m_high;
}

void Result::setHigh(double high)
{
    m_high = high;
}

double Result::low() const
{
    return m_low;
}

void Result::setLow(double low)
{
    m_low = low;
}
