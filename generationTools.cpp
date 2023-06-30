#include <generationTools.h>
#include <QRandomGenerator>

QVector<int> GenerationTools::sample(int a, int b, int k)
{
    QVector<int> result;
    if (b-a+1 < k || a > b || k < 0)
        return result;

    QVector<int> seq;
    for (int i = a; i <= b; ++i)
        seq.append(i);

    int n = b-a+1;
    int randVal;
    for (int i = 0; i < k; ++i)
    {
        randVal = QRandomGenerator::system()->bounded(n-i);
        result.append(seq[randVal]);
        seq.swapItemsAt(randVal, n-i-1);
    }
    return result;
}

double GenerationTools::normal(double mean, double stddev, uint32_t seed)
{
    if (stddev < 0)
        stddev = -stddev;
    QRandomGenerator gen(seed);
    std::normal_distribution<double> dist(mean, stddev);
    return dist(gen);
}

double GenerationTools::normal(double mean, double stddev)
{
    if (stddev < 0)
        stddev = -stddev;
    std::normal_distribution<double> dist(mean, stddev);
    return dist(*QRandomGenerator::system());
}

int GenerationTools::bounded(int num, int a, int b)
{
    if (b < a)
        std::swap(a,b);
    return abs(num) % (b - a + 1) + a;
}

qreal GenerationTools::boundedDouble(int num, qreal a, qreal b, qreal precision)
{
    if (b < a)
        std::swap(a,b);
    int left = a / precision;
    int right = b / precision;
    return (abs(num) % (right - left + 1) + left) * precision;
}
