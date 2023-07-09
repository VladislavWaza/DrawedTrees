#ifndef GENOM_H
#define GENOM_H
#include <QString>
#include <QColor>
#include "turtle.h"

class Genom
{
private:
    uint8_t* m_bytes;

    //из дискретного распределения range выбирает случайный угол, роль случайной величины выполняют 2 байта генома
    int getAngle(int curByteNum, const QVector<int> &range) const;

    //выбирает число ветвей исходящих из узла
    int getNumberOfBranches(int curByteNum) const;

    //составляет дисретное распределение для работы функции getAngle
    void getAngleRange(QVector<int> &range) const;

    //записывает в стоку через запятую параметры ствола: длину, утоньшение, цвет конца ствола в формате RGB.
    //Параметры обрамляются фигурными скобками
    void getTrunkParams(QString &params, int curByteNum) const;

public:
    static const int m_size = 200; //должно делиться на 4

    //150 байт на правило для перевода междоузлия
    //5 байт на определение начала ствола
    //2 байта на величину удлиннения
    //6 байт на цвет ствола(4 младших разряда каждого байта) и

    static const int m_endOfRuleBlock = 150;
    static const int m_endOfFirstTrunkBlock = 155;
    static const int m_endOfLengtheningBlock = 157;
    static const int m_endOfTrunkColorBlock = 163;

    static const int m_bytePerNode = 25; //узел определяется 25 байтами
    static const int m_bytePerBranch = 5; //ответвление определяется 5 байтами


    Genom();
    Genom(const Genom &other);
    Genom(const uint8_t *ptr);
    ~Genom();

    Genom &operator= (const Genom &other);
    Genom &operator= (const uint8_t *ptr);

    //выводит массив генов
    void genom(uint8_t* ptr) const;

    //выдает правило трансляции для междоузлий
    //алфавит правила:
    //[ означает сохранить положение в стек
    //] выгрузить положение из стека
    //T означает ствол
    //в фигурных скобках перечислены параметры ствола, через запятую
    //первое это стартовая длина, она равна единице
    //второе это утоньшение - число от 0 до 255,
    //это умноженный на 100 процент на который должна уменьшиться толщина за одну итерацию прорисовки
    //например если число 42, то 0.42 - это процент
    //следующие 3 числа означают цвет в формате RGB, который ствол достигает по своему завершению
    //I означает междоузлие
    void rule(QString &str) const;

    void leaf(TurtlePath &path, double len, double angle = 0) const;
    void leaf(QString &str) const;

    //выбирает стартовый цвет ствола
    QColor trunkColor() const;

    //выбирает удлиннение ствола
    double lengthening() const;

    //скрещивает 2 генома, беря одну половину генов от одного родителя, а другую - от другого
    //затем вносит случайную мутацию в случайном гене
    void cross(const Genom &father, Genom& son) const;
};

#endif // GENOM_H
