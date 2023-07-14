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
    void on_randomTreeButton_clicked();

    void on_load_triggered();

    void on_save_triggered();

    void on_againButton_clicked();

private:
    Ui::MainWindow *ui;
    LoadWindow *loadWindow;
    QPixmap *pixmap;
    Genom *genom;
};
#endif // MAINWINDOW_H
