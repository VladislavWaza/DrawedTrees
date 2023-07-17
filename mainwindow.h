#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "loadwindow.h"
#include "genom.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void genomeLoaded(uint8_t *bytes);

    void firstParentLoaded(uint8_t *bytes);

    void secondParentLoaded(uint8_t *bytes);

    void on_randomTreeButton_clicked();

    void on_load_triggered();

    void on_save_triggered();

    void on_againButton_clicked();

    void on_curFirst_triggered();

    void on_curSecond_triggered();

    void on_crossButton_clicked();

    void on_chooseFirst_triggered();

    void on_chooseSecond_triggered();

private:
    Ui::MainWindow *ui;
    LoadWindow *loadWindow;
    QPixmap *pixmap;
    Genom *genom;
    Genom *firstParent;
    Genom *secondParent;
    bool firstParentWasChosen;
    bool secondParentWasChosen;

    void activateAgainButton();
};
#endif // MAINWINDOW_H
