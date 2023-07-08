#include <QPainter>
#include <QDebug>
#include <QRandomGenerator>
#include "treeDrawing.h"
#include "turtle.h"
#include "generationTools.h"


void drawLine(QPainter &painter, TurtlePath &turtle, const QPen &pen, double len);
void growTree(QString &axiom, const QString &rule, int n, double lengthening);
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


void drawTree(QPixmap& returnedPixmap, const Genom &genom,
              double stddevForStandardRotate, double stddevForSmallRotate, double stddevForTrunkLength)
{
    QPixmap pixmap(1100, 1100);
    pixmap.fill();
    QPainter painter;
    painter.begin(&pixmap);
    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap);
    painter.setPen(pen);
    QPointF startPoint(550, 1050);
    TurtlePath turtle(startPoint);

    QString axiom = "I";
    QList<TurtleData> stack;
    TurtleData data;
    data.m_width = 30;
    int numberОfGrowthIterations = 3;
    double minWidth = 3;
    double leafWidth = 5;
    double startLen = 40;
    double pieceOfTrunkLen = 1;
    double leafLen = 15;
    QList<QColor> leafColors = {QColorConstants::Green, QColorConstants::DarkYellow, QColorConstants::DarkGreen};
    //важные параметры:
    //цвет листьев
    //толщины
    //форма листа

    QString rule;
    genom.rule(rule);
    data.setColor(genom.trunkColor());
    double lengthening = genom.lengthening();
    qDebug() << rule;
    qDebug() << lengthening;
    growTree(axiom, rule, numberОfGrowthIterations, lengthening);
    qDebug() << axiom.length();

    QChar ch;
    for (int i = 0; i < axiom.size(); ++i)
    {
        ch = axiom[i];
        if (ch == 'I')
        {
            pen.setColor(leafColors[QRandomGenerator::system()->bounded(leafColors.size())]);
            pen.setWidthF(leafWidth);
            drawLine(painter, turtle, pen, leafLen);
        }
        if (ch == 'T')
        {
            QString paramStr;
            i += 2; //переходим на символ после открывающей скобки
            while (axiom[i] != '}') //выписываем все параметры
            {
                paramStr += axiom[i];
                ++i;
            }
            QStringList paramList = paramStr.split(',');

            double len = startLen * paramList[0].toDouble() * GenerationTools::normal(1, stddevForTrunkLength);

            double thinning = 1 - paramList[1].toDouble() / 10000;
            //делить на 100 чтобы перевести в проценты
            //делать еще не 100 чтобы перевести в множитель
            //после вычитания из единицы получается величина которую нужно умножать на старую толщину чтобы получить новую

            double red = (paramList[2].toDouble() - data.m_red) / static_cast<int>(len / pieceOfTrunkLen);
            double green = (paramList[3].toDouble() - data.m_green) / static_cast<int>(len / pieceOfTrunkLen);
            double blue = (paramList[4].toDouble() - data.m_blue) / static_cast<int>(len / pieceOfTrunkLen);
            //величины, на которые нужно увеличивать цвет(по каждой из компонент) на каждом кусочке чтобы получить заданный


            //отрисовывать будем не все сразу, а равными кусочками
            double drawnLength = 0;
            while(drawnLength < len)
            {
                data.m_red += red;
                data.m_green += green;
                data.m_blue += blue;
                pen.setColor(data.getColor());

                data.m_width *= thinning;
                if (data.m_width < minWidth)
                    data.m_width = minWidth;
                pen.setWidthF(data.m_width);

                turtle.leftRotate(GenerationTools::normal(0, stddevForSmallRotate));
                drawLine(painter, turtle, pen, pieceOfTrunkLen);
                drawnLength += pieceOfTrunkLen;

                if (len - drawnLength < pieceOfTrunkLen) //дорисуем то что осталось
                {
                    drawLine(painter, turtle, pen, len - drawnLength);
                    break;
                }
            }
        }
        if (ch == '[')
        {
            data.set(turtle.getAngle(), turtle.currentPosition());
            stack.append(data);
        }
        if (ch == ']')
        {
            data.set(stack.last());
            stack.removeLast();
            turtle.setAngle(data.m_angle);
            turtle.moveTo(data.m_point);
        }
        if (ch == '(')
        {
            QString str;
            ++i;
            while (axiom[i] != ')')
            {
                str += axiom[i];
                ++i;
            }
            turtle.leftRotate(GenerationTools::normal(str.toInt(), stddevForStandardRotate));
        }
    }
    painter.end();
    returnedPixmap = pixmap.scaled(550, 550);
}


void drawLine(QPainter &painter, TurtlePath &turtle, const QPen &pen, double len)
{
    QPointF curPos = turtle.currentPosition();
    turtle.clear();
    turtle.moveTo(curPos);
    turtle.drawLine(len);
    painter.setPen(pen);
    painter.drawPath(turtle);
}

void growTree(QString &axiom, const QString &rule, int n, double lengthening)
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
