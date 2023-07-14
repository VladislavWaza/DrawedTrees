#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QDialog>
#include <QVBoxLayout>

namespace Ui {
class LoadWindow;
}

class LoadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadWindow(QWidget *parent = nullptr);
    ~LoadWindow();

private slots:

private:
    Ui::LoadWindow *ui;
    QVBoxLayout *boxLayout;
};

#endif // LOADWINDOW_H
