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
signals:
    void clicked(QObject *obj);
    void loaded(uint8_t *bytes);
private slots:
    void genomeClicked(QObject *obj);
    void on_loadButton_clicked();
    void on_deleteButton_clicked();

private:
    Ui::LoadWindow *ui;
    QVBoxLayout *boxLayout;
    QLabel *selectedLabel;
    QPalette *defaultPalette;
    QPalette *selectPalette;

    bool removeLine(const QString &fileName, int lineNumber);
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
};

QString previewName(QString dateTime);

#endif // LOADWINDOW_H
