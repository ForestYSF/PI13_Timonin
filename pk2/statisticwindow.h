#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "profilewindow.h"
#include <QVector>
#include <QHash>
class ProfileWindow;
namespace Ui {
class StatisticWindow;
}

class StatisticWindow : public QDialog
{
    Q_OBJECT
private:
    QVector<QString> users;
    QHash<QPushButton*, ProfileWindow*> mOpenUserProfile;
public:
    explicit StatisticWindow(QWidget *parent = nullptr);
    ~StatisticWindow();
    void onUpdateStats(ProfileWindow* pw, bool isOff);
private slots:
    void on_ShowUser();
private:
    Ui::StatisticWindow *ui;
};

#endif // STATISTICWINDOW_H
