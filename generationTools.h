#ifndef GENERATIONTOOLS_H
#define GENERATIONTOOLS_H
#include <QVector>

namespace GeneretionTools
{
    //возвращает набор без повторений из k значений выбранных в границах от a до b включая
    QVector<int> sample(int a, int b, int k);
}


#endif // GENERATIONTOOLS_H
