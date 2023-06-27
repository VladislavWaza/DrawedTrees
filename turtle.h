#ifndef TURTLE_H
#define TURTLE_H

#include <QPainterPath>

class TurtlePath : public QPainterPath
{
private:
    qreal m_angle;
public:
    TurtlePath();
    TurtlePath(const QPointF &startPoint);
    TurtlePath(const QPainterPath &path);

    void leftRotate(qreal angle);
    void rightRotate(qreal angle);
    void setAngle(qreal angle);
    qreal getAngle();
    void drawLine(qreal len);
};

#endif // TURTLE_H
