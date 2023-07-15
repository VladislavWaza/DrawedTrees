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
    selectedLabel = nullptr;
    defaultPalette = nullptr;
    selectPalette = nullptr;

    QFile file("./saves/genoms.csv");
    if (!file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly))
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл!");
    }
    else
    {
        QTextStream in(&file);
        QString line;
        QStringList splitedLine;
        QPixmap preview;
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

            //задаем палитры
            if (defaultPalette == nullptr)
            {
                defaultPalette = new QPalette;
                selectPalette = new QPalette;
                *defaultPalette = dateTimeLabel->palette();
                *selectPalette = *defaultPalette;
                selectPalette->setColor(QPalette::Window, QColor::fromRgb(250, 255, 255));
            }

            //устанавливаем обработчик событий и соединяем сигналы и слоты
            connect(this, &LoadWindow::clicked, this, &LoadWindow::genomeClicked);
            dateTimeLabel->installEventFilter(this);

            //читаем файл построчно
            line = in.readLine();
            splitedLine = line.split(',');

            dateTimeLabel->setText(splitedLine[0]); //выводим дата-время сохранения

            QString pixmapName = previewName(splitedLine[0]); //составляем имя превью

            //загружаем превью
            if(!preview.load(pixmapName))
            {
                QMessageBox::critical(this, "Ошибка!", "Не удалось загрузить превью дерева!");
            }
            else
            {
                previewLabel->setPixmap(preview); //выводим превью
            }               
        }
        file.close();
    }
}

LoadWindow::~LoadWindow()
{
    delete ui;
    delete boxLayout;
    if (defaultPalette != nullptr)
        delete defaultPalette;
    if (selectPalette != nullptr)
        delete selectPalette;
}

void LoadWindow::genomeClicked(QObject *obj)
{
    //Если какой-нибудь геном уже выбирали, то привести его к дефолтному виду
    if (selectedLabel != nullptr)
        selectedLabel->setPalette(*defaultPalette);

    selectedLabel = reinterpret_cast<QLabel*>(obj); //запомнить выбранный геном
    selectedLabel->setPalette(*selectPalette); //поменять его вид

    //разблокировать кнопки
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

void LoadWindow::on_loadButton_clicked()
{
    QFile file("./saves/genoms.csv");
    if (!file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly))
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл!");
    }
    else
    {
        QTextStream in(&file);
        QString dateTime = selectedLabel->text(); //извлекаем дата-время выбранного генома
        QString line;
        QStringList splitedLine;
        while (!in.atEnd())
        {
            line = in.readLine(); //читаем файл построчно
            splitedLine = line.split(',');

            if (dateTime == splitedLine[0]) //если нашли выбранный геном
            {
                //копируем геном
                uint8_t *bytes = new uint8_t[Genom::m_size];
                for (int i = 0; i < Genom::m_size; ++i)
                    bytes[i] = splitedLine[i+1].toInt();

                emit loaded(bytes); //передаем геном в MainWindow
                delete[] bytes;
                break;
            }
        }
        file.close();
    }
    this->close(); //закрываем окно
}


void LoadWindow::on_deleteButton_clicked()
{
    //блокируем кнопки
    ui->loadButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);

    QFile file("./saves/genoms.csv");
    if (!file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly))
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл!");
    }
    else
    {
        QTextStream in(&file);
        QString dateTime = selectedLabel->text(); //извлекаем дата-время выбранного генома
        int lineNumber = 0;
        bool flagFileClosed = false;
        QString line;
        while (!in.atEnd())
        {
            line = in.readLine(); //читаем файл построчно
            if (line.contains(dateTime)) //находим выбранный геном
            {
                file.close();
                flagFileClosed = true;
                if(!removeLine("./saves/genoms.csv", lineNumber)) //запускаем процесс удаления строки из csv
                   QMessageBox::critical(this, "Ошибка!", "Удаление генома не удалось!");
                break;
            }
            ++lineNumber;
        }
        if (!flagFileClosed)
        {
            QMessageBox::critical(this, "Ошибка!", "Удаление генома не удалось!");
            file.close();
        }

        //удаляем превью
        QString pixmapName = previewName(dateTime);
        if(!QFile::remove(pixmapName))
           QMessageBox::critical(this, "Ошибка!", "Удаление превью не удалось!");
    }
    selectedLabel->parent()->deleteLater(); //удаляем отображение генома из окна
    selectedLabel = nullptr; //выбранного генома больше нет
}

bool LoadWindow::removeLine(const QString &fileName, int lineNumber)
{
    QVector<QString> text;
    QFile file(fileName);
    if (!file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly))
        return false;
    else
    {
        QTextStream in(&file);
        QString line;
        while (!in.atEnd())
        {
            line = in.readLine(); //считываем построчно
            text.append(line); //сохраняем строки файла
        }
        file.close();

        if (text.size() < lineNumber)
            return false;
        text.remove(lineNumber); //удаляем выбранную строку

        if (!file.open(QIODeviceBase::Text | QIODeviceBase::WriteOnly | QIODeviceBase::ExistingOnly | QIODeviceBase::Truncate))
            return false;
        else
        {
            QTextStream out(&file);
            for (int i = 0; i < text.size(); ++i)
                out << text[i] << '\n'; //построчно выводим в очищенный файл
            file.close();
            return true;
        }
    }
}


QString previewName(QString dateTime)
{
    QString pixmapName = "./saves/TREE_";
    for (int i = 0; i < dateTime.size(); ++i)
    {
        if (dateTime[i] == '.' || dateTime[i] == ':')
            pixmapName.append('_');
        else
            pixmapName.append(dateTime[i]);
    }
    pixmapName += ".png";
    return pixmapName;
}
