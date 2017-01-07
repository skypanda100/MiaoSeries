#ifndef SIMULATE
#define SIMULATE

#include <QString>

class Simulate{

public:
    Simulate();
    ~Simulate();

    QString operation() const;
    void setOperation(const QString &operation);

    QString date() const;
    void setDate(const QString &date);

    double open() const;
    void setOpen(double open);

    int volume() const;
    void setVolume(int volume);

    double money() const;
    void setMoney(double money);

private:
    QString m_operation;
    QString m_date;
    double m_open;
    int m_volume;
    double m_money;
};

#endif // SIMULATE

