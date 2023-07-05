#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(
        ui->addWidget_button, &QPushButton::clicked,
        this, &MainWindow::onAddWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//функция чтения из файла (полного или рандомного текста)
QString MainWindow::readFile(QString filename, bool isRandom, bool isNick)
{
    QFile file(filename); // создаем объект класса QFile
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return ""; // если это сделать невозможно, то возвращаем пустое значение
    if (isRandom) {
        int sum = 1;
        while (!file.atEnd()) {
            file.readLine();
            sum++;
        }
        file.seek(0);
        int value;
        while (data == "") {
            if (isNick){
                while (usednicknames.contains(value)) value = QRandomGenerator::global()->bounded(sum);
                usednicknames.append(value);
            }
            else value = QRandomGenerator::global()->bounded(sum);
            while (value) {
                value--;
                data = file.readLine();
            }
        }
        data.replace("\n", "");
        data.replace("\r", "");
    }
    else {
        data = file.readAll(); //считываем все данные с файла в объект data
    }
    file.close();
    return data;
}

//функция обновления новостной ленты
void MainWindow::onAddWidget() {
    QString buttonText = readFile("C:/Games/qtprojects/pk2/news_list.txt", 1, 0); //получаем случайную тему новости
    CommentsWindow* cw = new CommentsWindow(this);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->vlayout->layout()); //указатель на вертикальный компоновщик
    QHBoxLayout* newLayout = new QHBoxLayout(ui->vlayout->parentWidget());  //указать на горизонтальный компоновщик

    QPushButton* button = new QPushButton(buttonText,ui->vlayout->parentWidget());  //создаём кнопку с текстом случайной темы
    newLayout->addWidget(button);       //сохраняем её в горизонтальный компоновщик
    QPushButton* cross_button = new QPushButton("Удалить", ui->vlayout->parentWidget());   //создаётся кнопка-крестик
    QPushButton* comments_button = new QPushButton("", ui->vlayout->parentWidget());   //создаётся кнопка-крестик

    QSize sz = button->size();
    sz = sz / (2,1);
    cross_button->setMaximumSize(sz);
    comments_button->setMaximumSize(sz);


    QPixmap pixmap_cross("C:/Games/qtprojects/pk2/cross_icon.png");
    QIcon cross_icon(pixmap_cross);
    cross_button->setIcon(cross_icon);

    QPixmap pixmap_comments("C:/Games/qtprojects/pk2/comments_icon.png");
    QIcon comments_icon(pixmap_comments);
    comments_button->setIcon(comments_icon);

    newLayout->addWidget(comments_button);
    newLayout->addWidget(cross_button);     //сохраняем её в горизонтальный компоновщик
    layout->insertLayout(0, newLayout);     //сохраняем горизонтальный компоновщик в вертикальном

    mButtonToLayoutMap.insert(cross_button, QPair<QHBoxLayout*,CommentsWindow*>(newLayout, cw)); //вставляем ключ-значение в хэш таблицу
    mCommentsHash.insert(comments_button, cw);
    mCommentsHashU.insert(cw, comments_button);
    //устанавливаем связь между нажатием на кнопку-крестик и вызовом функции удаления виджетов
    cw->setWindowTitle(buttonText);
    QObject::connect(
        cross_button, &QPushButton::clicked,
        this, &MainWindow::onRemoveWidget);
    QObject::connect(
        comments_button, &QPushButton::clicked,
        this, &MainWindow::onShowCommentsWindow);
    connect(cw, SIGNAL(_addComment()), this, SLOT(on_checkCountComments()));
}
//функция удаления темы в новостной ленте
void MainWindow::onRemoveWidget() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QPair<QHBoxLayout*,CommentsWindow*> pair = mButtonToLayoutMap.take(button);
    QHBoxLayout* layout = pair.first;
    CommentsWindow* cw = pair.second;

    cw->setAttribute(Qt::WA_DeleteOnClose);
    cw->close();
    while (layout->count() != 0) {
        QLayoutItem* item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
    delete layout;
}
//функция отображение окна с комментариями под постом
void MainWindow::onShowCommentsWindow() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    CommentsWindow* cw = mCommentsHash.value(button);
    cw->show();
}
//функция отображения счётчика комментариев под постом
void MainWindow::on_checkCountComments() {
    CommentsWindow* cw = qobject_cast<CommentsWindow*>(sender());
    QPushButton* button = mCommentsHashU.value(cw);
    int count = cw->onCheckNewsCount() + 1;
    if (button) button->setText(QString::number(count));
}
//функция создания рандомного пользователя, создание окна ProfileWindow
void MainWindow::onCreateUsersWindow() {
    QString Username = readFile("C:/Games/qtprojects/pk2/nickname_list.txt", 1, 1);

    int toxic = QRandomGenerator::global()->bounded(100);
    int pns = QRandomGenerator::global()->bounded(100);
    int sociability = QRandomGenerator::global()->bounded(100);
    int popularity = QRandomGenerator::global()->bounded(100);

    ProfileWindow* pw = new ProfileWindow(this, toxic, pns, sociability, popularity, Username);
}
//генерация заданное количество пользователей пользователей
void MainWindow::on_generateUsers_button_clicked()
{
    QProgressBar* progressBar = this->findChild<QProgressBar*>("usersCountBar");
    int k = progressBar->value();
    for (int i = 0; i < k; i++) onCreateUsersWindow();
}

//отображение окна статистики
void MainWindow::on_openStatistic_button_clicked()
{
    sw->show();
}

//если происходит изменение значения скорости
void MainWindow::on_progressBar_valueChanged(int value)
{
    QList<ProfileWindow*> pw_list = this->findChildren<ProfileWindow*>();
    int windowCount = pw_list.count();
    ProfileWindow* pw = nullptr;
    for (int i = 0; i < windowCount; i++) {
        pw = pw_list.at(i);
        pw->speed = value;
        pw->onUpdateTimer();
    }
}

//сигнал кнопки паузы
void MainWindow::on_pushButton_clicked()
{
    QList<ProfileWindow*> pw_list = this->findChildren<ProfileWindow*>();
    int windowCount = pw_list.count();
    ProfileWindow* pw = nullptr;
    for (int i = 0; i < windowCount; i++) {
        pw = pw_list.at(i);
        if (isPause) pw->onUpdateTimer();
        else pw->onPause();
    }
    isPause = !isPause;
}
