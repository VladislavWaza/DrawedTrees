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

void Genom::genom(uint8_t *ptr)
{
    for (int i = 0; i < m_size; ++i)
        ptr[i] = m_bytes[i];
}

void Genom::rule(QString &str)
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

    str += "T{1}?"; //рисуем ствол и обозначем символом '?' узел
    //2 БАЙТА НА ПЕРВУЮ ПАЛКУ (60 и 61)

    QString buffer;
    int branchesNum = 0;
    int curByteNum = 0;
    int randVar = 0;
    const int bytePerNode = 10; //узел определяется 10 байтами
    const int bytePerBranch = 2; //ответвление определяется 2 байтами
    char flagHasNodes = 1; //показывает есть ли еще незаполненные узлы

    while (flagHasNodes)
    {
        flagHasNodes = 0;
        if (curByteNum > m_sizeOfRuleBlock - bytePerNode) //досрочный выход при выходе из отведенного блока генов
            break;
        for (int i = 0; i < str.size(); ++i) //проходимся по строке
        {
            if (str[i] != '?' || curByteNum > m_sizeOfRuleBlock - bytePerNode) //пропуск узлов при выходе из отведенного блока генов
                buffer += str[i];
            else //на месте узла создаем новые ответвления
            {
                flagHasNodes= 1;
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

                    if ((m_bytes[curByteNum] & 1) == 1) //младший бит определяет тип ветки
                        buffer += "T{1}?";
                    else
                        buffer += 'I';


                    buffer += ']';
                    curByteNum += bytePerBranch;
                }
                curByteNum += bytePerNode - bytePerBranch * branchesNum; //пропусаем неиспользованные байти узла
            }
        }
        str = buffer;
        buffer.clear();
    }
    str = str.replace('?', 'I'); //заменяем оставшиеся узлы на листы
    qDebug() << curByteNum;
}

double Genom::lengthening()
{
    //2 байта на удлинение
    uint16_t gene = *reinterpret_cast<uint16_t*>(&m_bytes[m_sizeOfRuleBlock + 2]);
    uint8_t twoBits = 0;
    double lengthening = 1.1; //наименьшая величина
    for (int i = 0; i < 8; ++i) //16 бит разбиваем на 8 кусков по 2 бита
    {
        twoBits = ((gene >> 2*i) & 3);
        if (twoBits == 1)
            lengthening += 0.02;
        if (twoBits == 2)
            lengthening += 0.06;
        if (twoBits == 3)
            lengthening += 0.12;
    }
    return lengthening;
}

QColor Genom::trunkColor()
{
    //12 байт, младшие разряды
    //среднее арифметическое младших разрядов двух байт - одна буква в hex форме
    int i = m_sizeOfRuleBlock + 4;

    int red = ((m_bytes[i] & 15) + (m_bytes[i + 1] & 15)) / 2;
    i=i+2;
    red = 16*red + ((m_bytes[i] & 15) + (m_bytes[i + 1] & 15)) / 2;

    int green = ((m_bytes[i] & 15) + (m_bytes[i + 1] & 15)) / 2;
    i=i+2;
    green = 16*green + ((m_bytes[i] & 15) + (m_bytes[i + 1] & 15)) / 2;

    int blue = ((m_bytes[i] & 15) + (m_bytes[i + 1] & 15)) / 2;
    i=i+2;
    blue = 16*blue + ((m_bytes[i] & 15) + (m_bytes[i + 1] & 15)) / 2;

    QColor color(red, green, blue);
    return color;
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
