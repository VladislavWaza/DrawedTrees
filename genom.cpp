#include "genom.h"
#include <QRandomGenerator>
#include <QTime>

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
    int minStrSize = bounded(m_bytes[0], 3, 15);
    int countOfTrunks =  bounded(m_bytes[1], 1, minStrSize / 5 + 1);
    int countOfInternode = minStrSize - countOfTrunks;
    int countOfBrackets = 0;

    if (countOfTrunks != 0)
    {
        str += 'T';
        --countOfTrunks;
    }

    if (countOfTrunks != 0 && (m_bytes[1] >> 7 & 1))
    {
        str += 'T';
        --countOfTrunks;
    }

    int i = 2;
    while (countOfTrunks != 0 || countOfInternode != 0)
    {
        int randVar;
        if (countOfTrunks == 0 && countOfInternode != 0)
        {
            if (countOfBrackets > 0)
                randVar = bounded(m_bytes[i], 0, 6);
            else
                randVar = bounded(m_bytes[i], 0, 5);

            if (randVar % 3 == 1)
                str += "+";
            if (randVar % 3 == 2)
                str += "-";

            if (randVar / 3 == 0)
            {
                str += "[";
                ++countOfBrackets;
            }
            if (randVar / 3 == 1)
            {
                str += "I";
                --countOfInternode;
            }
            if (randVar / 3 == 2)
            {
                str += "]";
                --countOfBrackets;
            }
        }
        if (countOfTrunks != 0 && countOfInternode == 0)
        {
            if (countOfBrackets > 0)
                randVar = bounded(m_bytes[i], 0, 6);
            else
                randVar = bounded(m_bytes[i], 0, 5);

            if (randVar % 3 == 1)
                str += "+";
            if (randVar % 3 == 2)
                str += "-";

            if (randVar / 3 == 0)
            {
                str += "[";
                ++countOfBrackets;
            }
            if (randVar / 3 == 1)
            {
                str += "T";
                --countOfTrunks;
            }
            if (randVar / 3 == 2)
            {
                str += "]";
                --countOfBrackets;
            }
        }
        if (countOfTrunks != 0 && countOfInternode != 0)
        {
            if (countOfBrackets > 0)
                randVar = bounded(m_bytes[i], 0, 9);
            else
                randVar = bounded(m_bytes[i], 0, 8);

            if (randVar % 3 == 1)
                str += "+";
            if (randVar % 3 == 2)
                str += "-";

            if (randVar / 3 == 0)
            {
                str += "[";
                ++countOfBrackets;
            }
            if (randVar / 3 == 1)
            {
                str += "T";
                --countOfTrunks;
            }
            if (randVar / 3 == 2)
            {
                str += "I";
                --countOfInternode;
            }
            if (randVar / 3 == 3)
            {
                str += "]";
                --countOfBrackets;
            }
        }
        ++i;
    }

    while (countOfBrackets != 0)
    {
        str += "]";
        --countOfBrackets;
    }

    qDebug() << i;
}
