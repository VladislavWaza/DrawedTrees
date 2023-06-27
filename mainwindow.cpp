#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turtle.h"
#include "genom.h"
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
    QRandomGenerator generator(QTime::currentTime().msecsSinceStartOfDay());


    QString axiom = "I";
    QList<TurtleData> stack;
    TurtleData data;
    int n = 6;
    qreal angle = 20;
    qreal diffAngle = 10;
    qreal diffAngle2 = 5;
    qreal width = 30;
    qreal minWidth = 3;
    qreal leafWidth = 5;
    qreal len = 4;
    qreal leafLen = 5;

    QMap<QString, QString> translate;
    translate["T"] = "!T!T";
    translate["I"] = "T[T[+I][-I]I][++I][-I]";


    unsigned char ptr[40] = {179, 48, 157, 212, 241, 124, 147, 201, 97, 32, 139, 204, 230, 105,
                             4, 194, 205, 123, 242, 254, 90, 215, 51, 176, 164, 171, 67, 248, 6, 246, 179, 82, 200, 116, 85, 23, 230, 183, 42, 105};

    Genom genom;
    //genom = ptr;
    genom.getGenom(ptr);
    QDebug deb = qDebug();
    for (int i = 0; i < 40; ++i)
        deb.nospace() << static_cast<int>(ptr[i]) << ", ";

    genom.getRule(translate["I"]);
    qDebug() << translate["I"];

    QString bufAxiom;
    for (int i = 0; i < n; ++i)
    {
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

    qDebug() << axiom.length();
    QChar ch;
    for (int i = 0; i < axiom.size(); ++i)
    {
        ch = axiom[i];
        if (ch == 'I')
        {
            int randVal = bounded(generator.generate(), 1, 4);
            if (randVal == 1)
                pen.setColor(QColorConstants::Green);
            else if (randVal == 2)
                pen.setColor(QColorConstants::DarkYellow);
            else
                pen.setColor(QColorConstants::DarkGreen);
            pen.setWidthF(leafWidth);
            drawLine(painter, turtle, pen, leafLen);
        }
        if (ch == 'T' || ch == 'S')
        {
            turtle.leftRotate(boundedDouble(generator.generate(), -diffAngle2, diffAngle2, 0.01));

            pen.setColor(QColorConstants::Black);
            pen.setWidthF(width);
            drawLine(painter, turtle, pen, boundedDouble(generator.generate(), len / 2, len * 1.5, 0.1));
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

            turtle.leftRotate(angle + boundedDouble(generator.generate(), -diffAngle, diffAngle, 0.01));
        }
        if (ch == '+')
        {
            turtle.rightRotate(angle + boundedDouble(generator.generate(), -diffAngle, diffAngle, 0.01));
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
