#ifndef TREEDRAWING_H
#define TREEDRAWING_H
#include <QPixmap>
#include "genom.h"

struct StandardDeviations
{
    double plannedRotate;
    double unplannedRotate;
    double trunkLength;
    double antennaRotate;

    StandardDeviations &operator=(const StandardDeviations &other);
    static StandardDeviations byDefault();
};



void drawTree(QPixmap& returnedPixmap, const Genom &genom, const struct StandardDeviations &stddevs);

#endif // TREEDRAWING_H
