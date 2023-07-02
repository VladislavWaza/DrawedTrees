#ifndef GENOM_H
#define GENOM_H
#include<QString>
#include<QColor>

class Genom
{
private:
    uint8_t* m_bytes;
public:
    static const int m_size = 76; //должно делиться на 4
    //60 байт на правило для перевода междоузлия
    //2 байта на определение начала ствола
    //2 байта на величину удлиннения
    //12 байт на цвет ствола(4 младших разряда каждого байта) и

    static const int m_sizeOfRuleBlock = 60;
    Genom();
    Genom(const Genom &other);
    Genom(const uint8_t *ptr);
    ~Genom();

    Genom &operator= (const Genom &other);
    Genom &operator= (const uint8_t *ptr);

    void genom(uint8_t* ptr);
    void rule(QString &str);
    QColor trunkColor();
    double lengthening();
    Genom cross(const Genom &father);
};

#endif // GENOM_H
