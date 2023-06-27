#ifndef GENOM_H
#define GENOM_H
#include<QString>


int bounded(int num, int a, int b);
qreal boundedDouble(int num, qreal a, qreal b, qreal precision);

class Genom
{
private:
    unsigned char* m_bytes;
public:
    const int m_size = 40;
    Genom();
    Genom(const Genom &other);
    Genom(const unsigned char *ptr);
    ~Genom();

    Genom &operator= (const Genom &other);
    Genom &operator= (const unsigned char *ptr);

    void getGenom(unsigned char* ptr);
    void getRule(QString &str);
};

#endif // GENOM_H
