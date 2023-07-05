#ifndef TREEDRAWING_H
#define TREEDRAWING_H
#include <QPoint>
#include <QColor>
#include <QPainter>
#include "turtle.h"

namespace TreeDrawing
{
    void drawLine(QPainter &painter, TurtlePath &turtle, const QPen &pen, double len);

    void growTree(QString &axiom, const QString &rule, int n, double lengthening);
}

struct TurtleData
{
    double m_angle;
    QPointF m_point;
    double m_width;
    double m_red;
    double m_green;
    double m_blue;

    TurtleData();
    TurtleData(double angle, QPointF point, double width, double red, double green, double blue);

    void set(double angle, QPointF point, double width, double red, double green, double blue);
    void set(double angle, QPointF point);
    void set(struct TurtleData data);
    void setColor(const QColor& color);

    QColor getColor();
};


#endif // TREEDRAWING_H
