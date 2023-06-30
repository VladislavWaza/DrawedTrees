#include "genom.h"
#include "generationTools.h"
#include <QRandomGenerator>
#include <QTime>

Genom::Genom()
{
    m_bytes = new uint8_t[m_size];
    for (int i = 0; i < m_size/4; ++i)
        *reinterpret_cast<uint32_t*>(&m_bytes[4*i]) = QRandomGenerator::system()->generate();
}

Genom::Genom(const Genom &other)
{
    m_bytes = new uint8_t[m_size];
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = other.m_bytes[i];
}

Genom::Genom(const uint8_t *ptr)
{
    m_bytes = new uint8_t[m_size];
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

Genom &Genom::operator=(const uint8_t *ptr)
{
    for (int i = 0; i < m_size; ++i)
        m_bytes[i] = ptr[i];
    return *this;
}

void Genom::getGenom(uint8_t *ptr)
{
    for (int i = 0; i < m_size; ++i)
        ptr[i] = m_bytes[i];
}

void Genom::getRule(QString &str)
{
    str.clear();
    //формируем дискретное распределения для угла поворота
    //числа в диапазоне [-120, -90) повторятся 1 раз
    //числа в диапазоне [-90, -60) повторятся 2 раза
    //и тд
    //числа в диапазоне [-5, 6) повторятся 6 раз
    //числа в диапазоне [6, 16) повторятся 5 раз
    //и тд
    //числа в диапазоне [91, 121) повторятся 1 раз
    QVector<int> rangeBoundaries = {-120, -90, -60, -30, -15, -5, 6, 16, 31, 61, 91, 121};
    QVector<int> angles;
    for (int i = 1; i < rangeBoundaries.size(); ++i)
    {
        for (int angle = rangeBoundaries[i-1]; angle < rangeBoundaries[i]; ++angle)
        {
            int times = 0;
            if (i < rangeBoundaries.size() - i)
                times = i;
            else
                times = rangeBoundaries.size() - i;
            for (int j = 0; j < times; ++j)
                angles.append(angle);
        }
    }

    str += "T?"; //рисуем ствол и обозначем символом '?' узел
    QString buffer;
    int branchesNum = 0;
    int curByteNum = 0;
    int randVar = 0;
    int passСounter = 1;
    const int bytePerNode = 10; //узел определяется 10 байтами
    const int bytePerBranch = 2; //ответвление определяется 2 байтами
    while (passСounter <= 2)
    {
        for (int i = 0; i < str.size(); ++i) //проходимся по строке
        {
            if (str[i] != '?')
                buffer += str[i];
            else //на месте узла создаем новые ответвления
            {
                QList<int> branchesNumCounter(5);
                branchesNumCounter.fill(0);
                for (int j = 0; j < bytePerNode; ++j) //считаем кол-во байт дающих определенный остаток
                {
                    ++branchesNumCounter[m_bytes[curByteNum + j] % 5];
                }
                branchesNum = 0;
                int maxBranchesNumCounter = 0;
                //за кол-во ветвей возьмем наиболее часто встречающийся остаток + 1
                //при равенстве берется наимаеньшее число ветвей
                for (int j = 0; j < branchesNumCounter.size(); ++j)
                {
                    if (branchesNumCounter[j] > maxBranchesNumCounter)
                    {
                        maxBranchesNumCounter = branchesNumCounter[j];
                        branchesNum = j + 1;
                    }
                }

                for (int j = 0; j < branchesNum; ++j) //рисуем новые отростки
                {
                    buffer += "[(";
                    randVar = *reinterpret_cast<uint16_t*>(&m_bytes[curByteNum]);
                    randVar = GenerationTools::bounded(randVar, 0, angles.size() - 1); //выбираем угол
                    buffer += QString::number(angles[randVar]);
                    buffer += ')';

                    randVar = GenerationTools::bounded(m_bytes[curByteNum], 0, 8);
                    if (randVar / 3 == 0)
                        buffer += "T?";
                    if (randVar / 3 >= 1)
                        buffer += 'I';
                    buffer += ']';
                    curByteNum += bytePerBranch;
                }
                curByteNum += bytePerNode - bytePerBranch * branchesNum; //пропусаем неиспользованные байти узла
            }
        }
        str = buffer;
        buffer.clear();
        ++passСounter;
    }
    str = str.replace('?', 'I'); //заменяем оставшиеся узлы на листы
    qDebug() << curByteNum;
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
