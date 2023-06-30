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
    int minLenAxiom = 600;
    qreal stddevForStandardRotate = 0; //7.5
    qreal stddevForSmallRotate = 0; //2.5
    qreal stddevForTrunkLength = 0; //0.25
    qreal width = 5;
    qreal minWidth = 3;
    qreal leafWidth = 5;
    qreal len = 30;
    qreal leafLen = 30;
    QList<QColor> leafColors = {QColorConstants::Green, QColorConstants::DarkYellow, QColorConstants::DarkGreen};

    QMap<QString, QString> translate;
    translate["T"] = "!T!T";
    translate["I"] = "TT-[-I+I+I]+[+I-I-I]";

    uint8_t ptr[60] = {83, 139, 252, 47, 126, 112, 45, 105, 100, 28, 255, 150, 64, 209, 238, 82, 86, 203,
                       229, 53, 75, 251, 236, 24, 217, 134, 17, 52, 189, 249, 170, 52, 135, 3, 55, 121, 82,
                       10, 208, 103, 155, 23, 194, 38, 245, 76, 125, 230, 28, 35, 206, 202, 113, 34, 255, 138, 88, 45, 111, 182};
    uint8_t ptr2[60] = {58, 45, 21, 58, 9, 175, 28, 56, 191, 101, 154, 73, 221, 195, 239, 167, 70, 214, 196, 156, 227, 7,
                        214, 84, 55, 92, 215, 224, 152, 69, 31, 119, 31, 213, 66, 135, 124, 194, 160, 109, 64, 206, 250,
                        100, 105, 19, 139, 105, 18, 223, 175, 164, 121, 247, 139, 96, 123, 64, 251, 191};
    Genom genom2(ptr2);
    Genom genom(ptr);

    genom = genom.cross(genom2);

    genom.getGenom(ptr);
    QDebug deb = qDebug();
    for (int i = 0; i < Genom::m_size; ++i)
        deb.nospace() << static_cast<int>(ptr[i]) << ", ";

    genom.getRule(translate["I"]);
    qDebug() << translate["I"];


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
               bufAxiom += axiom[j];
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
        if (ch == 'T' || ch == 'S')
        {
            turtle.leftRotate(GenerationTools::normal(0, stddevForSmallRotate));
            pen.setColor(QColorConstants::Black);
            pen.setWidthF(width);
            drawLine(painter, turtle, pen, len * GenerationTools::normal(1, stddevForTrunkLength));
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
