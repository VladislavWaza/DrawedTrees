#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include "genom.h"

namespace Ui {
class LoadWindow;
}

class LoadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadWindow(QWidget *parent = nullptr);
    ~LoadWindow();

    void getGenome(uint8_t **ptr);
signals:
    void clicked(QObject *obj);
private slots:
    void genomeClicked(QObject *obj);
private:
    Ui::LoadWindow *ui;
    QVBoxLayout *boxLayout;
    uint8_t *bytes;
    QLabel *selectedLabel;
    QPalette *defaultPalette;
    QPalette *selectPalette;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // LOADWINDOW_H
