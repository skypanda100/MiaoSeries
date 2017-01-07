#include "simulate.h"

Simulate::Simulate(){

}

Simulate::~Simulate(){

}

QString Simulate::operation() const
{
    return m_operation;
}

void Simulate::setOperation(const QString &operation)
{
    m_operation = operation;
}

QString Simulate::date() const
{
    return m_date;
}

void Simulate::setDate(const QString &date)
{
    m_date = date;
}

double Simulate::open() const
{
    return m_open;
}

void Simulate::setOpen(double open)
{
    m_open = open;
}

int Simulate::volume() const
{
    return m_volume;
}

void Simulate::setVolume(int volume)
{
    m_volume = volume;
}

double Simulate::money() const
{
    return m_money;
}

void Simulate::setMoney(double money)
{
    m_money = money;
}
