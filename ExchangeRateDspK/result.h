#ifndef RESULT
#define RESULT

#include <QString>

class Result{
public:
    Result();
    ~Result();

    QString date() const;
    void setDate(const QString &date);

    double open() const;
    void setOpen(double open);

    double close() const;
    void setClose(double close);

    double high() const;
    void setHigh(double high);

    double low() const;
    void setLow(double low);

private:
    QString m_date;
    double m_open;
    double m_close;
    double m_high;
    double m_low;
};

#endif // RESULT

