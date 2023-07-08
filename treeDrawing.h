#ifndef TREEDRAWING_H
#define TREEDRAWING_H
#include <QPixmap>
#include "genom.h"

void drawTree(QPixmap& returnedPixmap, const Genom &genom,
              double stddevForStandardRotate, double stddevForSmallRotate, double stddevForTrunkLength);

#endif // TREEDRAWING_H
