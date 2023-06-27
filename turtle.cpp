#include "turtle.h"
#include <QtMath>

TurtlePath::TurtlePath()
    :QPainterPath(), m_angle(90)
{
}

TurtlePath::TurtlePath(const QPointF &startPoint)
    :QPainterPath(startPoint), m_angle(90)
{

}

TurtlePath::TurtlePath(const QPainterPath &path)
    :QPainterPath(path), m_angle(90)
{

}

void TurtlePath::leftRotate(qreal angle)
{
    m_angle += angle;
    if (m_angle >= 360)
        m_angle = fmod(m_angle, 360);
    if (m_angle < 0)
        m_angle = fmod(m_angle, 360) + 360;
}

void TurtlePath::rightRotate(qreal angle)
{
    m_angle -= angle;
    if (m_angle >= 360)
        m_angle = fmod(m_angle, 360);
    if (m_angle < 0)
        m_angle = fmod(m_angle, 360) + 360;
}

void TurtlePath::setAngle(qreal angle)
{
    m_angle = angle;
    if (m_angle >= 360)
        m_angle = fmod(m_angle, 360);
    if (m_angle < 0)
        m_angle = fmod(m_angle, 360) + 360;
}

qreal TurtlePath::getAngle()
{
    return m_angle;
}

void TurtlePath::drawLine(qreal len)
{
    QPointF	point = currentPosition();
    qreal curX = point.x();
    qreal curY = point.y();
    qreal newX = curX + len*qCos(qDegreesToRadians(m_angle));
    qreal newY = curY - len*qSin(qDegreesToRadians(m_angle));
    lineTo(newX, newY);
}

