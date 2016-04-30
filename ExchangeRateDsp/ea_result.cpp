#include "ea_result.h"

ExchangeRateResult::ExchangeRateResult(){

}

ExchangeRateResult::~ExchangeRateResult(){

}

void ExchangeRateResult::setName(QString name){
    this->m_name = name;
}

void ExchangeRateResult::setColor(int color){
    this->m_color = color;
}

void ExchangeRateResult::setXData(DoubleArray doubleArray){
    this->x_data = doubleArray;
}

void ExchangeRateResult::setYData(DoubleArray doubleArray){
    this->y_data = doubleArray;
}

QString ExchangeRateResult::getName(){
    return this->m_name;
}

int ExchangeRateResult::getColor(){
    return this->m_color;
}

DoubleArray ExchangeRateResult::getXData(){
    return this->x_data;
}

DoubleArray ExchangeRateResult::getYData(){
    return this->y_data;
}
