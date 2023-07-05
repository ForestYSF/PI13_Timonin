#ifndef COMMENTSWINDOW_H
#define COMMENTSWINDOW_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include "profilewindow.h"
#include <QSize>
#include <QHash>
#include <QPropertyAnimation>
class ProfileWindow;

namespace Ui {
class CommentsWindow;
}

class CommentsWindow : public QDialog
{
    Q_OBJECT

public:
    QHash<QPushButton*,ProfileWindow*> mPushUserImage;
public:
    explicit CommentsWindow(QWidget *parent = nullptr);
    ~CommentsWindow();

    void onAddComment(ProfileWindow* pw1, QString usermessage, ProfileWindow* pw2 = nullptr);
    int onCheckNewsCount();
private:
    Ui::CommentsWindow *ui;
private slots:
    void on_ShowUser();
signals:
    void _addComment();
};

#endif // COMMENTSWINDOW_H
