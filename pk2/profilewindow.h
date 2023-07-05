#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QDialog>
#include <QLabel>
#include "commentswindow.h"
#include "statisticwindow.h"
#include <QPixmap>
#include <qrandom.h>
#include <QHash>
#include <QList>
#include <QProgressBar>
class CommentsWindow;
namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QDialog
{
    Q_OBJECT
public:
    QString username;
    int popularity;
    int health;
    int pns;
    int speed = 1;
    QHash<QString,QPixmap> mProfileHash;
    QVector<QString> friends;
    QVector<QString> enemies;
private:
    int toxic;
    int sociability;
    QTimer *t_addcomment;
public:
    explicit ProfileWindow(QWidget *parent = nullptr, int toxic = 0, int pns = 0, int sociability = 0, int popularity = 0, QString username = "");
    ~ProfileWindow();
    void onShowCharacter();
    void onUpdateHealth();
    void onMockeryComment(ProfileWindow* pw, CommentsWindow* cw);
    void onUpdateTimer();
    void onPause();
private slots:
    void onAddComment();
private:
    Ui::ProfileWindow *ui;
    void onUpdateList(QString list_name, ProfileWindow* pw, bool isDelete);
};

#endif // PROFILEWINDOW_H
