#include "ea_result.h"

ExchangeRateResult::ExchangeRateResult(){

}

ExchangeRateResult::~ExchangeRateResult(){

}

QString ExchangeRateResult::date() const
{
    return m_date;
}

void ExchangeRateResult::setDate(const QString &date)
{
    m_date = date;
}

double ExchangeRateResult::open() const
{
    return m_open;
}

void ExchangeRateResult::setOpen(double open)
{
    m_open = open;
}

double ExchangeRateResult::close() const
{
    return m_close;
}

void ExchangeRateResult::setClose(double close)
{
    m_close = close;
}

double ExchangeRateResult::high() const
{
    return m_high;
}

void ExchangeRateResult::setHigh(double high)
{
    m_high = high;
}

double ExchangeRateResult::low() const
{
    return m_low;
}

void ExchangeRateResult::setLow(double low)
{
    m_low = low;
}
