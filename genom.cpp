#include "genom.h"
#include "generationTools.h"
#include <QRandomGenerator>
#include <QTime>

Genom::Genom()
{
    m_bytes = new unsigned char[m_size];
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = QRandomGenerator::system()->bounded(UCHAR_MAX+1);
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
    /*
    сперва выберем число букв
    затем сгенерируем последовательность из символов [ ] I, но такую чтобы скобки открывались и закрывались корректно
    затем пройдем по последовательности и будем менять некоторые I на T,
    но так чтобы T не шло после I на одном скобочном уровне, то есть чтобы Trunk не крепился поверх Internode
    */
    str.clear();
    int T_E_S_T = m_bytes[0] + m_bytes[1] + m_bytes[2] + m_bytes[3];
    int countOfLetters = GenerationTools::bounded(T_E_S_T, 3, 10); // тут сделать нормальное распределение
    int countOfBrackets = 0;
    str += 'T';
    --countOfLetters;

    unsigned char randVar;
    int i = 1;
    while (countOfLetters > 0)
    {
        if (i == m_size)
        if (countOfBrackets == 0)
            randVar = GenerationTools::bounded(m_bytes[i], 0, 5);
        else
            randVar = GenerationTools:: bounded(m_bytes[i], 0, 8);
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

Genom Genom::cross(const Genom &father)
{
    QVector<int> vec = GenerationTools::sample(0, m_size-1, m_size/2); // номера байт которые будут унаследованы от отца
    Genom son(*this);
    for (int i = 0; i < m_size/2; ++i)
        son.m_bytes[vec[i]] = father.m_bytes[vec[i]];
    son.m_bytes[QRandomGenerator::system()->bounded(m_size)] = QRandomGenerator::system()->bounded(UCHAR_MAX+1); // мутация
    return son;
}
