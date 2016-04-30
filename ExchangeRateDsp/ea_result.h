#ifndef EA_RESULT
#define EA_RESULT

#include "memblock.h"
#include <QString>

class ExchangeRateResult{

public:
    ExchangeRateResult();
    ~ExchangeRateResult();

    void setName(QString);
    void setColor(int);
    void setXData(DoubleArray);
    void setYData(DoubleArray);
    QString getName();
    int getColor();
    DoubleArray getXData();
    DoubleArray getYData();

private:
    QString m_name;
    int m_color;
    DoubleArray y_data;
    DoubleArray x_data;
};

#endif // EA_RESULT

