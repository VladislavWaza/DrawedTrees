#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treeDrawing.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);      
    pixmap = new QPixmap;
    genom = new Genom;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pixmap;
    delete genom;
}

void MainWindow::on_load_triggered()
{
    loadWindow = new LoadWindow;
    loadWindow->exec();
    uint8_t *bytes = new uint8_t[Genom::m_size];
    uint8_t *duplicate = bytes;
    loadWindow->getGenome(&bytes);
    if (bytes != nullptr)
    {
        *genom = bytes;
        on_againButton_clicked();
    }
    delete[] duplicate;
    delete loadWindow;
}


void MainWindow::on_save_triggered()
{
    QFile file("./saves/genoms.csv");

    if (!QDir("saves").exists()) //все файлы будут в папке saves
        QDir().mkdir("saves");

    if (!file.open(QIODeviceBase::Text | QIODeviceBase::WriteOnly | QIODeviceBase::Append))
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось создать и/или открыть файл!");
    }
    else
    {
        //в csv сохраняется дата-время и геном дерева
        QTextStream out(&file);
        QString dateTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        out << dateTime;
        uint8_t bytes[Genom::m_size];
        genom->genom(bytes);
        for (int i = 0; i < Genom::m_size; ++i)
            out << ',' << static_cast<int>(bytes[i]);
        out << '\n';
        file.close();

        //в формате png сохраняется превью дерева с именем соответствующем времени сохранения
        QString pixmapName = "./saves/TREE_";
        for (int i = 0; i < dateTime.size(); ++i)
        {
            if (dateTime[i] == '.' || dateTime[i] == ':')
                pixmapName.append('_');
            else
                pixmapName.append(dateTime[i]);
        }
        pixmapName += ".png";

        QPixmap preview = pixmap->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        if(!preview.save(pixmapName))
        {
            QMessageBox::critical(this, "Ошибка!", "Не удалось сохранить превью дерева!");
        }
    }
}

void MainWindow::on_randomTreeButton_clicked()
{
    delete genom;
    genom = new Genom;
    on_againButton_clicked();
}

void MainWindow::on_againButton_clicked()
{
    StandardDeviations stddevs;
    stddevs.plannedRotate = 0; //7.5
    stddevs.unplannedRotate = 0; //1
    stddevs.trunkLength = 0; //0.25
    stddevs.antennaRotate = 0; //0.1

    drawTree(*pixmap, *genom, stddevs);
    ui->label->setPixmap(pixmap->scaled(550, 550, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

