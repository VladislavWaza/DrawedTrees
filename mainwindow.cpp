#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turtle.h"
#include "genom.h"
#include <generationTools.h>
#include <QPainter>
#include <QColor>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QTime>

struct TurtleData
{
    TurtleData()
        :m_angle(0), m_point(0, 0), m_width(0) {}
    TurtleData(qreal angle, QPointF point, qreal width)
        :m_angle(angle), m_point(point), m_width(width) {}
    qreal m_angle;
    QPointF m_point;
    qreal m_width;
    void set(qreal angle, QPointF point, qreal width)
    {
        m_angle = angle;
        m_point = point;
        m_width = width;
    }
    void set(struct TurtleData data)
    {
        m_angle = data.m_angle;
        m_point = data.m_point;
        m_width = data.m_width;
    }
};

void drawLine(QPainter &painter, TurtlePath &turtle, const QPen &pen, qreal len)
{
    QPointF curPos = turtle.currentPosition();
    turtle.clear();
    turtle.moveTo(curPos);
    turtle.drawLine(len);
    painter.setPen(pen);
    painter.drawPath(turtle);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(1000, 1000);
    pixmap.fill();
    QPainter painter;
    painter.begin(&pixmap);
    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap);
    painter.setPen(pen);
    QPointF startPoint(500, 990);
    TurtlePath turtle(startPoint);

    QString axiom = "I";
    QList<TurtleData> stack;
    TurtleData data;
    int n = 3;
    int minLenAxiom = 1000;
    qreal stddevForStandardRotate = 0; //7.5
    qreal stddevForSmallRotate = 0; //10
    qreal stddevForTrunkLength = 0; //0.25
    qreal width = 3;
    qreal minWidth = 3;
    qreal leafWidth = 5;
    double startLen = 30;
    qreal leafLen = 15;
    QList<QColor> leafColors = {QColorConstants::Green, QColorConstants::DarkYellow, QColorConstants::DarkGreen};

    //важные параметры:
    //цвет листьев
    //смещение цвета ствола
    //толщины
    //форма листа

    QMap<QString, QString> translate;
    translate["I"] = "";

    uint8_t ptr[Genom::m_size] = {39, 35, 162, 91, 197, 132, 13, 99, 231, 9, 200, 84, 97, 254, 230, 0, 68, 101, 152, 57, 226, 212, 171, 207, 130, 189, 235, 16, 210, 195, 35, 155, 183, 92, 204, 8, 162, 174, 146, 9, 159, 167, 82, 112, 67, 207, 27, 216, 142, 81, 99, 84, 240, 65, 250, 206, 204, 200, 219, 209, 87, 233, 184, 44, 15, 15, 60, 35, 8, 8, 115, 54, 8, 8, 7, 65};
    uint8_t ptr2[Genom::m_size] = {39, 35, 162, 226, 197, 132, 33, 99, 231, 9, 200, 84, 97, 164, 230, 0, 68, 125, 152, 57, 226, 60, 171, 232, 130, 189, 44, 92, 219, 195, 35, 155, 183, 92, 204, 8, 215, 107, 99, 209, 159, 167, 82, 112, 67, 207, 27, 216, 114, 81, 99, 84, 240, 65, 250, 206, 204, 200, 11, 209, 87, 233, 184, 44, 15, 15, 60, 35, 8, 8, 204, 141, 8, 8, 7, 35};
    Genom genom2(ptr);
    Genom genom(ptr);
    genom = genom.cross(genom2);

    genom.genom(ptr);
    QDebug deb = qDebug();
    for (int i = 0; i < Genom::m_size; ++i)
        deb.nospace() << static_cast<int>(ptr[i]) << ", ";

    genom.rule(translate["I"]);
    QColor trunkColor = genom.trunkColor();
    double lengthening = genom.lengthening();
    qDebug() << translate["I"];
    qDebug() << lengthening;

    QString bufAxiom;
    int m = 0;
    for (int i = 0; i < n; ++i)
    //while (axiom.size() < minLenAxiom)
    {
        ++m;
        bufAxiom.clear();
        for (int j = 0; j < axiom.length(); ++j)
        {
            if (translate.find(axiom[j]) != translate.end())
            {
                bufAxiom += translate[axiom[j]];
            }
            else
            {
                bufAxiom += axiom[j];
                if (axiom[j] == 'T')
                {
                    QString str;
                    j += 2;
                    while (axiom[j] != '}')
                    {
                        str += axiom[j];
                        ++j;
                    }
                    bufAxiom += '{' + QString::number(str.toDouble() * lengthening) + '}';
                }
            }
        }
        axiom = bufAxiom;
    }
    qDebug() << m;

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
            pen.setColor(trunkColor);
            pen.setWidthF(width);

            QString str;
            i += 2;
            while (axiom[i] != '}')
            {
                str += axiom[i];
                ++i;
            }
            double len = startLen * str.toDouble() * GenerationTools::normal(1, stddevForTrunkLength);

            //отрисовывать будем не все сразу, а равными кусочками
            double drawnLength = 0;
            for (int pieceNumber = 0; pieceNumber < len / startLen; ++pieceNumber)
            {
                turtle.leftRotate(GenerationTools::normal(0, stddevForSmallRotate));
                drawLine(painter, turtle, pen, startLen);
                drawnLength += startLen;
            }
            //дорисуем то что осталось
            drawLine(painter, turtle, pen, len - drawnLength);
        }
        if (ch == '[')
        {
            data.set(turtle.getAngle(), turtle.currentPosition(), width);
            stack.append(data);
        }
        if (ch == ']')
        {
            data.set(stack.last());
            stack.removeLast(); 
            turtle.setAngle(data.m_angle);
            turtle.moveTo(data.m_point);
            width = data.m_width;
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
        if (ch == '!')
        {
            width -= 0.25;
            if (width < minWidth)
                width = minWidth;
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
