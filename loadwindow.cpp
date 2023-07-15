#include <QMessageBox>
#include <QGroupBox>
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
    bytes = nullptr;
    selectedLabel = nullptr;
    defaultPalette = new QPalette;
    selectPalette = new QPalette;

    QFile file("./saves/genoms.csv");
    if (!file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly))
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл!");
    }
    else
    {
        QTextStream in(&file);
        while (!in.atEnd()) //читаем файл до конца
        {
            //добавляем виджеты
            QGroupBox *row = new QGroupBox(ui->scrollArea);
            row->setFixedSize(300, 100);
            QLabel *dateTimeLabel = new QLabel(row);
            dateTimeLabel->setFixedSize(200, 100);
            dateTimeLabel->setAlignment(Qt::AlignCenter);
            dateTimeLabel->move(100, 0);
            dateTimeLabel->setAutoFillBackground(true);
            QLabel *previewLabel = new QLabel(row);
            previewLabel->setFixedSize(100, 100);
            ui->scrollArea->widget()->layout()->addWidget(row);

            //устанавливаем обработчик событий и соединяем сигналы и слоты
            connect(this, &LoadWindow::clicked, this, &LoadWindow::genomeClicked);
            dateTimeLabel->installEventFilter(this);

            QString line = in.readLine(); //читаем файл построчно
            QStringList splitedLine = line.split(',');
            dateTimeLabel->setText(splitedLine[0]);

            //составляем имя превью
            QString pixmapName = "./saves/TREE_";
            for (int i = 0; i < splitedLine[0].size(); ++i)
            {
                if (splitedLine[0][i] == '.' || splitedLine[0][i] == ':')
                    pixmapName.append('_');
                else
                    pixmapName.append(splitedLine[0][i]);
            }
            pixmapName += ".png";

            //загружаем превью
            QPixmap preview;
            if(!preview.load(pixmapName))
            {
                QMessageBox::critical(this, "Ошибка!", "Не удалось загрузить превью дерева!");
            }
            else
            {
                previewLabel->setPixmap(preview);
            }

            //задем палитры
            *defaultPalette = dateTimeLabel->palette();
            *selectPalette = *defaultPalette;
            selectPalette->setColor(QPalette::Window, QColor::fromRgb(250, 255, 255));
            //sample_palette.setColor(QPalette::WindowText, Qt::blue);
        }
        file.close();
    }
}

LoadWindow::~LoadWindow()
{
    delete ui;
    delete boxLayout;
    delete defaultPalette;
    delete selectPalette;
    if (bytes != nullptr)
        delete bytes;
}

void LoadWindow::getGenome(uint8_t **ptr)
{
    if (bytes != nullptr)
    {
        for (int i = 0; i < Genom::m_size; ++i)
            *ptr[i] = bytes[i];
    }
    else
        *ptr = nullptr;
}

void LoadWindow::genomeClicked(QObject *obj)
{
    if (selectedLabel != nullptr)
        selectedLabel->setPalette(*defaultPalette);
    selectedLabel = reinterpret_cast<QLabel*>(obj);
    selectedLabel->setPalette(*selectPalette);
    ui->loadButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

bool LoadWindow::eventFilter(QObject *obj, QEvent *ev)
{
    switch(ev->type())
    {
        case QEvent::MouseButtonRelease:
        {
            QMouseEvent *mouseEvent = reinterpret_cast<QMouseEvent*>(ev);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                emit clicked(obj);
                return true;
            }
            break;
        }
        default:;
    }
    return QObject::eventFilter(obj, ev);
}
