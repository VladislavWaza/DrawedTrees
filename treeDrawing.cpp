#include <QPainter>
#include <QDebug>
#include <QRandomGenerator>
#include "treeDrawing.h"
#include "generationTools.h"
#include "nib.h"

void drawLine(QPainter &painter, Nib &nib, double len)
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidthF(nib.getWidth());
    pen.setColor(nib.getColor());
    painter.setPen(pen);

    QPointF	start = nib.getPoint();
    double angle = nib.getAngle();
    double newX = start.x() + len*qCos(qDegreesToRadians(angle));
    double newY = start.y() - len*qSin(qDegreesToRadians(angle));
    QPointF end(newX, newY);
    painter.drawLine(start, end);

    nib.setPoint(end);
}
void growTree(QString &axiom, const QString &rule, int n, double lengthening);

void drawTree(QPixmap& returnedPixmap, const Genom &genom,
              double stddevForStandardRotate, double stddevForSmallRotate, double stddevForTrunkLength)
{
    QPixmap pixmap(1100, 1100);
    pixmap.fill();
    QPainter painter;
    painter.begin(&pixmap);

    QPointF startPoint(550, 1050);
    int numberОfGrowthIterations = 3;
    double startLen = 40;
    double pieceOfTrunkLen = 1;
    double pieceOfAntennaLen = 1;
    double minWidth = 3;
    double antennaWidth = 3;
    double startWidth = 30;
    QList<QColor> leafColors = {QColorConstants::Green, QColorConstants::DarkYellow, QColorConstants::DarkGreen};
    QString axiom = "I";
    QList<Nib> stackOfNibs;
    Nib nib(minWidth, startWidth, startPoint, 90);

    QString rule;
    genom.rule(rule);
    qDebug() << rule;

    QString leafRule;
    genom.leaf(leafRule);
    qDebug() << leafRule;

    double lengthening = genom.lengthening();
    nib.setColor(genom.trunkColor());

    growTree(axiom, rule, numberОfGrowthIterations, lengthening);
    qDebug() << axiom.length();


    ////////////////////////
    axiom.replace('I', leafRule);
    ////////////////////////
    TurtlePath leaf;
    genom.leaf(leaf, 10);
    ////////////////////////


    QChar ch;
    for (int i = 0; i < axiom.size(); ++i)
    {
        ch = axiom[i];
        if (ch == 'A')
        {
            QString paramStr;
            i += 2; //переходим на символ после открывающей скобки
            while (axiom[i] != '}') //выписываем все параметры
            {
                paramStr += axiom[i];
                ++i;
            }
            QStringList paramList = paramStr.split(',');
            int countOfPieces = paramList[0].toInt();
            double angle = paramList[1].toDouble();
            nib.setColor(paramList[2].toInt(), paramList[3].toInt(), paramList[4].toInt());
            nib.setWidth(antennaWidth);

            for (int j = 0; j < countOfPieces; ++j)
            {
                drawLine(painter, nib, pieceOfAntennaLen);
                nib.increaseAngle(angle);
            }
        }
        if (ch == 'I')
        {
            /*
            nib.setColor(leafColors[QRandomGenerator::system()->bounded(leafColors.size())]);
            nib.setWidth(antennaWidth);
            /////////////////////////
            painter.setPen(pen);
            genom.leaf(leaf, 15, turtle.getAngle());
            TurtlePath myleaf = leaf.translated(turtle.currentPosition());
            painter.drawPath(myleaf);
            ////////////////////////
            //drawLine(painter, turtle, pen, leafLen);
            */
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

            double red = (paramList[2].toDouble() - nib.getRed()) / static_cast<int>(len / pieceOfTrunkLen);
            double green = (paramList[3].toDouble() - nib.getGreen()) / static_cast<int>(len / pieceOfTrunkLen);
            double blue = (paramList[4].toDouble() - nib.getBlue()) / static_cast<int>(len / pieceOfTrunkLen);
            //величины, на которые нужно увеличивать цвет(по каждой из компонент) на каждом кусочке чтобы получить заданный

            //отрисовывать будем не все сразу, а равными кусочками
            double drawnLength = 0;
            while(drawnLength < len)
            {
                nib.increaseColor(red, green, blue);
                nib.multiplyWidth(thinning);
                nib.increaseAngle(GenerationTools::normal(0, stddevForSmallRotate));
                drawLine(painter, nib, pieceOfTrunkLen);
                drawnLength += pieceOfTrunkLen;
                if (len - drawnLength < pieceOfTrunkLen) //дорисуем то что осталось
                {
                    drawLine(painter, nib, len - drawnLength);
                    break;
                }
            }
        }
        if (ch == '[')
        {
            stackOfNibs.append(nib);
        }
        if (ch == ']')
        {
            nib.set(stackOfNibs.last());
            stackOfNibs.removeLast();
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
            nib.increaseAngle(GenerationTools::normal(str.toInt(), stddevForStandardRotate));
        }
    }
    painter.end();
    returnedPixmap = pixmap.scaled(550, 550);
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
