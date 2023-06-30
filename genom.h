#ifndef GENOM_H
#define GENOM_H
#include<QString>

class Genom
{
private:
    unsigned char* m_bytes;
public:
    static const int m_size = 40; //должно делиться на 4
    Genom();
    Genom(const Genom &other);
    Genom(const unsigned char *ptr);
    ~Genom();

    Genom &operator= (const Genom &other);
    Genom &operator= (const unsigned char *ptr);

    void getGenom(unsigned char* ptr);
    void getRule(QString &str);
    Genom cross(const Genom &father);
};

#endif // GENOM_H
