#ifndef GENOM_H
#define GENOM_H
#include<QString>

class Genom
{
private:
    uint8_t* m_bytes;
public:
    static const int m_size = 60; //должно делиться на 4
    Genom();
    Genom(const Genom &other);
    Genom(const uint8_t *ptr);
    ~Genom();

    Genom &operator= (const Genom &other);
    Genom &operator= (const uint8_t *ptr);

    void getGenom(uint8_t* ptr);
    void getRule(QString &str);
    Genom cross(const Genom &father);
};

#endif // GENOM_H
