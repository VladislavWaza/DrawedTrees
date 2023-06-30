#include "genom.h"
#include "generationTools.h"
#include <QRandomGenerator>
#include <QTime>

Genom::Genom()
{
    m_bytes = new unsigned char[m_size];
    for (int i = 0; i < m_size/4; ++i)
        *reinterpret_cast<unsigned int*>(&m_bytes[4*i]) = QRandomGenerator::system()->generate();
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
    str += "T?"; //рисуем ствол и обозначем символом '?' узел
    QString buffer;
    int passСounter = 1;
    int curByteNum = 0;
    int branchesNum = 0;
    unsigned char randVar = 0;
    while (passСounter <= 2)
    {
        for (int i = 0; i < str.size(); ++i) //проходимся по строке
        {
            if (str[i] != '?')
                buffer += str[i];
            else //на месте узла создаем новые ответвления
            {
                branchesNum = 0;
                for (int j = 0; j < 5; ++j)
                    branchesNum += m_bytes[curByteNum + j];
                branchesNum = GenerationTools::bounded(branchesNum / 5, 1, 5); //генерируем количество ответвлений

                for (int j = 0; j < branchesNum; ++j) //рисуем новые отростки
                {
                    buffer += '[';
                    randVar = GenerationTools::bounded(m_bytes[curByteNum], 0, 8);
                    ++curByteNum;
                    if (randVar % 3 == 1)
                        buffer += '+';
                    if (randVar % 3 == 2)
                        buffer += '-';
                    if (randVar / 3 == 0)
                        buffer += "T?";
                    if (randVar / 3 >= 1)
                        buffer += 'I';
                    buffer += ']';
                }
                curByteNum += branchesNum - 5;
            }
        }
        str = buffer;
        ++passСounter;
    }
    str = str.replace('?', 'I'); //заменяем оставшиеся узлы на листы
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
