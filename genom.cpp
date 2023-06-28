#include "genom.h"
#include <QRandomGenerator>
#include <QTime>
#include <QStack>

int bounded(int num, int a, int b)
{
    return abs(num) % (b - a + 1) + a;
}

qreal boundedDouble(int num, qreal a, qreal b, qreal precision)
{
    int left = a / precision;
    int right = b / precision;
    return (abs(num) % (right - left + 1) + left) * precision;
}

Genom::Genom()
{
    m_bytes = new unsigned char[m_size];
    QRandomGenerator generator(QTime::currentTime().msecsSinceStartOfDay());
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = generator.bounded(0, 256);
}

Genom::Genom(const Genom &other)
{
    m_bytes = new unsigned char[m_size];
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = other.m_bytes[i];
}

Genom::Genom(const unsigned char *ptr)
{
    m_bytes = new unsigned char[m_size];
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = ptr[i];
}

Genom::~Genom()
{
    delete[] m_bytes;
}

Genom &Genom::operator=(const Genom &other)
{
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = other.m_bytes[i];
    return *this;
}

Genom &Genom::operator=(const unsigned char *ptr)
{
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = ptr[i];
    return *this;
}

void Genom::getGenom(unsigned char *ptr)
{
    for (int i = 0; i < m_size; ++i)
        ptr[i] = m_bytes[i];
}

void Genom::getRule(QString &str)
{
    str.clear();
    int countOfLetters = bounded(m_bytes[0], 3, 10); // тут сделать нормальное распределение
    int countOfBrackets = 0;
    str += 'T';

    /*
    сперва выберем число букв
    затем сгенерируем последовательность из символов [ ] I, но такую чтобы скобки открывались и закрывались корректно
    затем пройдем по последовательности и будем менять некоторые I на T,
    но так чтобы T не шло после I в одном скобочном уровне, то есть чтобы Trunk не крепился поверх Internode
    */

    unsigned char randVar;
    int i = 1;
    while (countOfLetters > 0)
    {
        if (countOfBrackets == 0)
            randVar = bounded(m_bytes[i], 0, 5);
        else
            randVar = bounded(m_bytes[i], 0, 8);
        ++i;

        if (randVar % 3 == 1)
            str += "+";
        if (randVar % 3 == 2)
            str += "-";

        if (randVar / 3 == 0)
        {
            str += '[';
            ++countOfBrackets;
        }
        if (randVar / 3 == 1)
        {
            str += 'I';
            --countOfLetters;
        }
        if (randVar / 3 == 2)
        {
            str += ']';
            --countOfBrackets;
        }
    }
    while (countOfBrackets != 0)
    {
        str += "]";
        --countOfBrackets;
    }

    int internodeAppLevel = INT_MAX;
    for (int j = 0; j < str.size(); ++j)
    {
        if (str[j] == '[')
            ++countOfBrackets;
        if (str[j] == ']')
        {
            --countOfBrackets;
            if (internodeAppLevel > countOfBrackets)
                internodeAppLevel = INT_MAX;
        }
        if (str[j] == 'I' && m_bytes[i] % 2 == 0 && internodeAppLevel > countOfBrackets)
        {
            str[j] = 'T';
            if (countOfBrackets < internodeAppLevel)
                internodeAppLevel = countOfBrackets;
            ++i;
        }
    }
    qDebug() << i;
}
