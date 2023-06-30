#ifndef GENERATIONTOOLS_H
#define GENERATIONTOOLS_H
#include <QVector>

namespace GenerationTools
{
    //возвращает набор без повторений из k значений выбранных в границах от a до b включая
    QVector<int> sample(int a, int b, int k);

    //генерирует по сиду нормально распределенную случайную величину, если ско < 0, то берется модуль
    double normal(double mean, double stddev, uint32_t seed);

    //генерирует нормально распределенную случайную величину, если ско < 0, то берется модуль
    double normal(double mean, double stddev);

    //переводит num на отрезок [a,b]
    int bounded(int num, int a, int b);

    //переводит num на отрезок [a,b] делая его дробным
    qreal boundedDouble(int num, qreal a, qreal b, qreal precision);


}


#endif // GENERATIONTOOLS_H
