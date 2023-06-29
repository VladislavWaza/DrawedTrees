#include <generationTools.h>
#include <QRandomGenerator>

QVector<int> GeneretionTools::sample(int a, int b, int k)
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
