#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <QFile>
#include "loadwindow.h"
#include "ui_loadwindow.h"

LoadWindow::LoadWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadWindow),
    boxLayout(new QVBoxLayout)
{
    ui->setupUi(this);
    ui->scrollArea->widget()->setLayout(boxLayout);//добавляем в scrollArea объект boxLayout, который выстраивает виджеты по вертикали

    QFile file("./saves/genoms.csv");
    if (!file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly))
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл!");
    }
    else
    {
        QTextStream in(&file);
        while (!in.atEnd()) //читаем файл построчно
        {
            //добавляем виджеты
            QGroupBox *row = new QGroupBox(ui->scrollArea);
            row->setFixedSize(300, 100);
            QLabel *previewLabel = new QLabel(row);
            previewLabel->setFixedSize(100, 100);
            QLabel *dateTimeLabel = new QLabel(row);
            dateTimeLabel->setFixedSize(150, 100);
            dateTimeLabel->move(110, 0);
            ui->scrollArea->widget()->layout()->addWidget(row);

            //connect

            QString line = in.readLine();
            QStringList splitedLine = line.split(',');
            dateTimeLabel->setText(splitedLine[0]);

            QString pixmapName = "./saves/TREE_";
            for (int i = 0; i < splitedLine[0].size(); ++i)
            {
                if (splitedLine[0][i] == '.' || splitedLine[0][i] == ':')
                    pixmapName.append('_');
                else
                    pixmapName.append(splitedLine[0][i]);
            }
            pixmapName += ".png";

            QPixmap preview;
            if(!preview.load(pixmapName))
            {
                QMessageBox::critical(this, "Ошибка!", "Не удалось загрузить превью дерева!");
            }
            else
            {
                previewLabel->setPixmap(preview);
            }

        }
        file.close();
    }

}

LoadWindow::~LoadWindow()
{
    delete ui;
    delete boxLayout;
}

