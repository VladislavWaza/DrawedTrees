#ifndef NIB_H
#define NIB_H
#include <QPointF>
#include <QColor>

class Nib
{
    double m_minWidth;
    double m_angle;
    QPointF m_point;
    double m_width;
    double m_red;
    double m_green;
    double m_blue;
public:
    Nib(double minWidth);
    Nib(double minWidth, double width, const QPointF &point, double angle);

    void increaseWidth(double width);
    void multiplyWidth(double coeff);

    void increaseColor(double red, double green, double blue);
    void increaseAngle(double angle);

    void set(const Nib& other);
    void setColor(const QColor& color);
    void setColor(int red, int green, int blue);
    void setColor(double red, double green, double blue);
    void setWidth(double width);
    void setPoint(const QPointF& point);

    QColor getColor() const;
    QPointF getPoint() const;
    double getAngle() const;
    double getWidth() const;
    double getRed() const;
    double getGreen() const;
    double getBlue() const;

    //void set(double angle, QPointF point);
};

#endif // NIB_H
