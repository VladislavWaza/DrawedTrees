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
    //int n = 4;
    int minLenAxiom = 3000;
    qreal angle = 35;
    qreal stddevForStandardRotate = 7.5;
    qreal stddevForSmallRotate = 2.5;
    qreal stddevForTrunkLength = 0.25;
    qreal width = 5;
    qreal minWidth = 3;
    qreal leafWidth = 5;
    qreal len = 15;
    qreal leafLen = 15;
    QList<QColor> leafColors = {QColorConstants::Green, QColorConstants::DarkYellow, QColorConstants::DarkGreen};

    QMap<QString, QString> translate;
    translate["T"] = "!T!T";
    translate["I"] = "TT-[-I+I+I]+[+I-I-I]";

    unsigned char ptr[40] = {221, 229, 217, 254, 199, 150, 40, 62, 72, 75, 17, 90, 17, 190, 177, 233, 171,
                             154, 25, 71, 124, 134, 101, 10, 165, 147, 33, 245, 13, 122, 69, 31, 113, 137, 53, 34, 192, 122, 74, 69};
    unsigned char ptr2[40] = {101, 58, 55, 53, 207, 52, 97, 191, 237, 88, 184, 145, 35, 141, 221, 246, 62, 50, 116, 117,
                              188, 74, 84, 82, 115, 3, 157, 56, 245, 241, 21, 42, 21, 124, 15, 86, 107, 128, 157, 150};
    Genom genom2(ptr2);
    Genom genom;

    //genom = genom.cross(genom2);

    genom.getGenom(ptr);
    QDebug deb = qDebug();
    for (int i = 0; i < Genom::m_size; ++i)
        deb.nospace() << static_cast<int>(ptr[i]) << ", ";

    genom.getRule(translate["I"]);
    qDebug() << translate["I"];


    QString bufAxiom;
    //for (int i = 0; i < n; ++i)
    int n = 0;
    while (axiom.size() < minLenAxiom)
    {
        ++n;
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
    qDebug() << n;

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
        if (ch == '-')
        {

            turtle.leftRotate(GenerationTools::normal(angle, stddevForStandardRotate));
        }
        if (ch == '+')
        {

            turtle.rightRotate(GenerationTools::normal(angle, stddevForStandardRotate));
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
