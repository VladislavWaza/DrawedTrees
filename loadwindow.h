#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QDialog>

namespace Ui {
class LoadWindow;
}

class LoadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadWindow(QWidget *parent = nullptr);
    ~LoadWindow();

private:
    Ui::LoadWindow *ui;
};

#endif // LOADWINDOW_H
