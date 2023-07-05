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


void Genom::getAngleRange(QVector<int> &range)
{
    range.clear();
    //формируем дискретное распределения для угла поворота
    //числа в диапазоне [-120, -90) повторятся 1 раз
    //числа в диапазоне [-90, -60) повторятся 2 раза
    //и тд
    //числа в диапазоне [-5, 6) повторятся 6 раз
    //числа в диапазоне [6, 16) повторятся 5 раз
    //и тд
    //числа в диапазоне [91, 121) повторятся 1 раз
    QVector<int> rangeBoundaries = {-120, -90, -60, -30, -15, -5, 6, 16, 31, 61, 91, 121};
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
                range.append(angle);
        }
    }
}

int Genom::getAngle(int curByteNum, const QVector<int> &range)
{
    int randVar = *reinterpret_cast<uint16_t*>(&m_bytes[curByteNum]); //выписываем двухбайтное число
    randVar = GenerationTools::bounded(randVar, 0, range.size() - 1); //выбираем номер ячейки в range
    return range[randVar];
}


int Genom::getNumberOfBranches(int curByteNum)
{
    QList<int> counter(5);
    counter.fill(0);
    //считаем кол-во байт дающих определенный остаток, проходим по первым 2 байтам каждой ветки данного узла
    for (int j = 0; j < m_bytePerNode / m_bytePerBranch; ++j)
    {
        ++counter[m_bytes[curByteNum + j * m_bytePerBranch] % 5];
        ++counter[m_bytes[curByteNum + j * m_bytePerBranch + 1] % 5];
    }
    int numberOfBranches = 0;
    int maxCounter = 0;
    //за кол-во ветвей возьмем наиболее часто встречающийся остаток + 1
    //при равенстве берется наименьшее число ветвей
    for (int j = 0; j < counter.size(); ++j)
    {
        if (counter[j] > maxCounter)
        {
            maxCounter = counter[j];
            numberOfBranches = j + 1;
        }
    }
    return numberOfBranches;
}


void Genom::getTrunkParams(QString &params, int curByteNum)
{
    params.clear();
    params += "{1,";

    //на утоньшение идут старшие 4 бита первого и второго байта ветки
    int thinning = m_bytes[curByteNum] / 16 * 16 + m_bytes[curByteNum + 1] / 16;
    params += QString::number(thinning) + ',';

    //на послесдвиговый цвет идут 3 байта
    params += QString::number(m_bytes[curByteNum + 2]) + ',';
    params += QString::number(m_bytes[curByteNum + 3]) + ',';
    params += QString::number(m_bytes[curByteNum + 4]) + "}";
}


void Genom::rule(QString &str)
{
    str.clear();   
    QVector<int> range;
    getAngleRange(range);

    //рисуем ствол и обозначем символом '?' узел
    str += 'T';
    QString params;
    getTrunkParams(params, 150); //первая часть ствола определяется байтами 150-154
    str += params;
    str += '?';


    QString buffer;
    int curByteNum = 0;   
    char flagHasNodes = 1; //показывает есть ли еще незаполненные узлы

    while (flagHasNodes)
    {
        flagHasNodes = 0;
        if (curByteNum > m_endOfRuleBlock - m_bytePerNode) //досрочный выход при выходе из отведенного блока генов
            break;
        for (int i = 0; i < str.size(); ++i) //проходимся по строке
        {
            if (str[i] != '?' || curByteNum > m_endOfRuleBlock - m_bytePerNode) //пропуск узлов при выходе из отведенного блока генов
                buffer += str[i];
            else //на месте узла создаем новые ответвления
            {
                flagHasNodes= 1;
                int numberOfBranches = getNumberOfBranches(curByteNum); //выбираем число ответвлений
                for (int j = 0; j < numberOfBranches; ++j) //рисуем новые отростки
                {
                    buffer += "[(";
                    buffer += QString::number(getAngle(curByteNum, range)); //выбираем угол поворота ветки
                    buffer += ')';

                    if ((m_bytes[curByteNum] & 1) == 1) //младший бит определяет тип ветки
                    {
                        buffer += 'T';
                        getTrunkParams(params, curByteNum);
                        buffer += params;
                        buffer += '?';
                    }
                    else
                        buffer += 'I';

                    buffer += ']';
                    curByteNum += m_bytePerBranch;
                }
                curByteNum += m_bytePerNode - m_bytePerBranch * numberOfBranches; //пропусаем неиспользованные байты узла
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
    uint16_t gene = *reinterpret_cast<uint16_t*>(&m_bytes[m_endOfFirstTrunkBlock]);
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
    int i = m_endOfLengtheningBlock;

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

void Genom::cross(const Genom &father, Genom& son)
{
    son = *this;
    QVector<int> vec = GenerationTools::sample(0, m_size-1, m_size/2); // номера байт которые будут унаследованы от отца
    for (int i = 0; i < m_size/2; ++i)
        son.m_bytes[vec[i]] = father.m_bytes[vec[i]];
    son.m_bytes[QRandomGenerator::system()->bounded(m_size)] = QRandomGenerator::system()->bounded(UCHAR_MAX+1); // мутация
}
