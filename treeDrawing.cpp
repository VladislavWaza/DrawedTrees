#include "treeDrawing.h"

void TreeDrawing::drawLine(QPainter &painter, TurtlePath &turtle, const QPen &pen, double len)
{
    QPointF curPos = turtle.currentPosition();
    turtle.clear();
    turtle.moveTo(curPos);
    turtle.drawLine(len);
    painter.setPen(pen);
    painter.drawPath(turtle);
}

void TreeDrawing::growTree(QString &axiom, const QString &rule, int n, double lengthening)
{
    QString bufAxiom;
    for (int i = 0; i < n; ++i)
    {
        bufAxiom.clear();
        for (int j = 0; j < axiom.length(); ++j)
        {
            if (axiom[j] == 'I')
                bufAxiom += rule;
            else if (axiom[j] == 'T')
            {
                bufAxiom += axiom[j];
                QString str;
                j += 2;
                while (axiom[j] != ',') //выписываем текущую длину ствола
                {
                    str += axiom[j];
                    ++j;
                }
                //удлинняем, а оставшиеся параметры переносим как есть
                bufAxiom += '{' + QString::number(str.toDouble() * lengthening) + ',';
            }
            else
                bufAxiom += axiom[j];
        }
        axiom = bufAxiom;
    }
}


















TurtleData::TurtleData()
    :m_angle(0), m_point(0, 0), m_width(0), m_red(0), m_green(0), m_blue(0) {}

TurtleData::TurtleData(double angle, QPointF point, double width, double red, double green, double blue)
    :m_angle(angle), m_point(point), m_width(width), m_red(red), m_green(green), m_blue(blue) {}

void TurtleData::set(double angle, QPointF point, double width, double red, double green, double blue)
{
    m_angle = angle;
    m_point = point;
    m_width = width;
    m_red = red;
    m_green = green;
    m_blue = blue;
}

void TurtleData::set(double angle, QPointF point)
{
    m_angle = angle;
    m_point = point;
}

void TurtleData::set(struct TurtleData data)
{
    m_angle = data.m_angle;
    m_point = data.m_point;
    m_width = data.m_width;
    m_red = data.m_red;
    m_green = data.m_green;
    m_blue = data.m_blue;
}

void TurtleData::setColor(const QColor& color)
{
    m_red = color.red();
    m_green = color.green();
    m_blue = color.blue();
}

QColor TurtleData::getColor()
{
    QColor color;
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
    color.setRed(m_red);
    color.setGreen(m_green);
    color.setBlue(m_blue);
    return color;
}
