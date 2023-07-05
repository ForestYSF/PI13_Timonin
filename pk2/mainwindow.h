#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QHash>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "commentswindow.h"
#include "profilewindow.h"
#include "statisticwindow.h"
#include <QTimer>
#include <QVector>
#include <QScrollArea>
#include <QCloseEvent>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QHash<QPushButton*,QPair<QHBoxLayout*,CommentsWindow*>> mButtonToLayoutMap; //хэш таблица для гориз. компоновщиков и доп.окна
    QHash<QPushButton*, CommentsWindow*> mCommentsHash;  //хэш таблица для работы с окнами комментариев
    QHash<CommentsWindow*,QPushButton*> mCommentsHashU;  //обратная хэш таблица для работы с окнами комментариев
    QVector<int> usednicknames;
    StatisticWindow* sw = new StatisticWindow(this);
    bool isPause = false;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString readFile(QString filename, bool isRandom, bool isNick);
    void onAddWidget(); // новые новостные поля
    void onRemoveWidget(); // удаление новостных полей
    void onShowCommentsWindow(); //отображение окна с комментариями под записью
    void onShowCommentsCount(); //отображение количества комментариев под записью
    void onCreateUsersWindow(); //создание окон пользователей (созданием самих пользователей)
private slots:
    void on_generateUsers_button_clicked();
    void on_openStatistic_button_clicked();
    void on_progressBar_valueChanged(int value);
    void on_pushButton_clicked();

public slots:
    void on_checkCountComments();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
