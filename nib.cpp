#include "nib.h"

Nib::Nib(double minWidth)
    :m_minWidth(minWidth){}

Nib::Nib(double minWidth, double width, const QPointF &point, double angle)
    :m_minWidth(minWidth), m_point(point)
{
    m_width = width;
    if (m_width < m_minWidth)
        m_width = m_minWidth;
    m_angle = angle;
    if (m_angle >= 360)
        m_angle = fmod(m_angle, 360);
    if (m_angle < 0)
        m_angle = fmod(m_angle, 360) + 360;
}

void Nib::increaseWidth(double width)
{
    m_width += width;
    if (m_width < m_minWidth)
        m_width = m_minWidth;
}

void Nib::multiplyWidth(double coeff)
{
    m_width *= coeff;
    if (m_width < m_minWidth)
        m_width = m_minWidth;
}

void Nib::increaseColor(double red, double green, double blue)
{
    m_red += red;
    m_green += green;
    m_blue += blue;
    if (m_red > 255)
        m_red = 255;
    if (m_red < 0)
        m_red = 0;
    if (m_green > 255)
        m_green = 255;
    if (m_green < 0)
        m_green = 0;
    if (m_blue > 255)
        m_blue = 255;
    if (m_blue < 0)
        m_blue = 0;
}

void Nib::increaseAngle(double angle)
{
    m_angle += angle;
    if (m_angle >= 360)
        m_angle = fmod(m_angle, 360);
    if (m_angle < 0)
        m_angle = fmod(m_angle, 360) + 360;
}

void Nib::set(const Nib &other)
{
    m_minWidth = other.m_minWidth;
    m_width = other.m_width;
    m_angle = other.m_angle;
    m_point = other.m_point;
    m_red = other.m_red;
    m_green = other.m_green;
    m_blue = other.m_blue;
}

void Nib::setColor(const QColor &color)
{
    m_red = color.red();
    m_green = color.green();
    m_blue = color.blue();
}

void Nib::setColor(int red, int green, int blue)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    if (m_red > 255)
        m_red = 255;
    if (m_red < 0)
        m_red = 0;
    if (m_green > 255)
        m_green = 255;
    if (m_green < 0)
        m_green = 0;
    if (m_blue > 255)
        m_blue = 255;
    if (m_blue < 0)
        m_blue = 0;
}

void Nib::setColor(double red, double green, double blue)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    if (m_red > 255)
        m_red = 255;
    if (m_red < 0)
        m_red = 0;
    if (m_green > 255)
        m_green = 255;
    if (m_green < 0)
        m_green = 0;
    if (m_blue > 255)
        m_blue = 255;
    if (m_blue < 0)
        m_blue = 0;
}

void Nib::setWidth(double width)
{
    m_width = width;
    if (m_width < m_minWidth)
        m_width = m_minWidth;
}

void Nib::setPoint(const QPointF &point)
{
    m_point = point;
}

QColor Nib::getColor() const
{
    QColor color(m_red, m_green, m_blue);
    return color;
}

QPointF Nib::getPoint() const
{
    return m_point;
}

double Nib::getAngle() const
{
    return m_angle;
}

double Nib::getWidth() const
{
    return m_width;
}

double Nib::getRed() const
{
    return m_red;
}

double Nib::getGreen() const
{
    return m_green;
}

double Nib::getBlue() const
{
    return m_blue;
}
