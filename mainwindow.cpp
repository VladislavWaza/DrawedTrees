#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turtle.h"
#include "genom.h"
#include "generationTools.h"
#include "treeDrawing.h"
#include <QPainter>
#include <QColor>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    int n = 4;
    double stddevForStandardRotate = 0; //7.5
    double stddevForSmallRotate = 0; //1
    double stddevForTrunkLength = 0; //0.25
    data.m_width = 20;
    double minWidth = 3;
    double leafWidth = 5;
    double startLen = 40;
    double pieceOfTrunkLen = 1;
    double leafLen = 15;
    QList<QColor> leafColors = {QColorConstants::Green, QColorConstants::DarkYellow, QColorConstants::DarkGreen};

    //важные параметры:
    //цвет листьев
    //смещение цвета ствола
    //толщины
    //форма листа

    uint8_t ptr[Genom::m_size] = {155, 187, 20, 5, 5, 136, 168, 255, 236, 28, 252, 114, 31, 141, 80, 88, 248, 58, 81, 247, 32, 97, 253, 47, 172, 106, 13, 70, 135, 238, 42, 199, 123, 233, 45, 176, 81, 255, 188, 129, 216, 179, 209, 45, 19, 40, 247, 216, 25, 68, 130, 243, 32, 51, 89, 165, 239, 247, 42, 124, 0, 154, 240, 62, 18, 48, 15, 103, 209, 249, 142, 78, 185, 121, 84, 162, 8, 18, 14, 75, 87, 244, 50, 194, 121, 146, 62, 217, 38, 134, 223, 122, 58, 229, 121, 60, 249, 22, 8, 133, 208, 211, 46, 99, 130, 27, 245, 177, 74, 62, 245, 217, 43, 18, 56, 219, 106, 149, 11, 56, 117, 170, 37, 125, 179, 246, 55, 211, 55, 93, 239, 134, 61, 111, 35, 62, 73, 77, 43, 111, 110, 118, 252, 55, 13, 161, 42, 75, 80, 204, 75, 160, 200, 200, 13, 161, 237, 150, 111, 77, 114, 186, 238, 34, 40, 218, 76, 237, 132, 173, 137, 112, 219, 163, 196, 25, 96, 37, 65, 81, 51, 21, 253, 30, 118, 247, 163, 16, 207, 183, 54, 122, 219, 159, 216, 98, 209, 31, 121, 208};
    uint8_t ptr2[Genom::m_size] = {155, 187, 20, 6, 224, 136, 210, 148, 36, 68, 244, 114, 224, 141, 102, 175, 248, 58, 81, 122, 32, 129, 110, 253, 172, 68, 247, 70, 240, 15, 89, 199, 123, 233, 123, 176, 81, 56, 181, 129, 200, 179, 253, 132, 19, 201, 112, 216, 25, 181, 130, 243, 32, 51, 98, 32, 170, 247, 150, 124, 79, 131, 240, 62, 212, 48, 15, 175, 144, 249, 142, 78, 233, 94, 170, 148, 194, 18, 134, 14, 87, 244, 249, 194, 121, 146, 236, 217, 32, 134, 147, 122, 58, 229, 32, 215, 100, 57, 77, 133, 208, 211, 46, 99, 18, 102, 60, 192, 74, 52, 84, 12, 138, 70, 150, 219, 106, 149, 199, 211, 117, 138, 16, 125, 193, 246, 55, 211, 124, 38, 239, 134, 96, 67, 35, 62, 73, 179, 43, 111, 103, 68, 252, 55, 13, 67, 42, 168, 80, 203, 75, 160, 179, 200, 196, 22, 237, 150, 111, 160, 114, 186, 111, 34, 40, 119, 11, 237, 71, 180, 137, 112, 219, 163, 196, 25, 63, 46, 65, 81, 51, 188, 253, 246, 118, 118, 163, 132, 207, 247, 54, 122, 118, 90, 97, 98, 209, 137, 54, 208};
    Genom genom2(ptr2);
    Genom genom(ptr2);
    //genom.cross(genom2, genom);

    genom.genom(ptr);
    QDebug deb = qDebug();
    for (int i = 0; i < Genom::m_size; ++i)
        deb.nospace() << static_cast<int>(ptr[i]) << ", ";

    QString rule;
    genom.rule(rule);
    data.setColor(genom.trunkColor());
    double lengthening = genom.lengthening();
    qDebug() << rule;
    qDebug() << lengthening;

    TreeDrawing::growTree(axiom, rule, n, lengthening);

    qDebug() << axiom.length();
    QChar ch;
    for (int i = 0; i < axiom.size(); ++i)
    {
        ch = axiom[i];
        if (ch == 'I')
        {
            pen.setColor(leafColors[QRandomGenerator::system()->bounded(leafColors.size())]);
            pen.setWidthF(leafWidth);
            TreeDrawing::drawLine(painter, turtle, pen, leafLen);
        }
        if (ch == 'T')
        {
            QString str;
            i += 2; //переходим на символ после открывающей скобки
            while (axiom[i] != ',') //выписываем длину ствола
            {
                str += axiom[i];
                ++i;
            }
            double len = startLen * str.toDouble() * GenerationTools::normal(1, stddevForTrunkLength);

            str.clear();
            ++i; //переходим на символ после запятой
            while (axiom[i] != ',') //выписываем утоньшение
            {
                str += axiom[i];
                ++i;
            }
            double thinning = 1 - str.toDouble() / 10000;
            //делить на 100 чтобы перевести в проценты
            //делать еще не 100 чтобы перевести в множитель
            //после вычитания из единицы получается величина которую нужно умножать на старую толщину чтобы получить новую

            str.clear();
            ++i; //переходим на символ после запятой
            while (axiom[i] != ',') //выписываем красный цвет
            {
                str += axiom[i];
                ++i;
            }
            double red = (str.toDouble() - data.m_red) / static_cast<int>(len / pieceOfTrunkLen);
            //то, на сколько нужно увеличивать цвет на каждом кусочке чтобы получить заданный

            str.clear();
            ++i; //переходим на символ после запятой
            while (axiom[i] != ',') //выписываем зеленый цвет
            {
                str += axiom[i];
                ++i;
            }
            double green = (str.toDouble() - data.m_green) / static_cast<int>(len / pieceOfTrunkLen);
            //то, на сколько нужно увеличивать цвет на каждом кусочке чтобы получить заданный

            str.clear();
            ++i; //переходим на символ после запятой
            while (axiom[i] != '}') //выписываем синий цвет
            {
                str += axiom[i];
                ++i;
            }
            double blue = (str.toDouble() - data.m_blue) / static_cast<int>(len / pieceOfTrunkLen);
            //то, на сколько нужно увеличивать цвет на каждом кусочке чтобы получить заданный

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
                TreeDrawing::drawLine(painter, turtle, pen, pieceOfTrunkLen);
                drawnLength += pieceOfTrunkLen;

                if (len - drawnLength < pieceOfTrunkLen) //дорисуем то что осталось
                {
                    TreeDrawing::drawLine(painter, turtle, pen, len - drawnLength);
                    break;
                }
            }
        }
        if (ch == '[')
        {
            data.set(turtle.getAngle(), turtle.currentPosition());
            stack.append(data);
            data.m_width /= 1.5;
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
    pixmap = pixmap.scaled(550, 550);
    ui->label->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}
