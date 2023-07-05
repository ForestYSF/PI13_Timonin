#include "profilewindow.h"
#include "ui_profilewindow.h"


ProfileWindow::ProfileWindow(QWidget *parent, int toxic, int pns, int sociability, int popularity, QString username) :
    QDialog(parent),
    ui(new Ui::ProfileWindow)
{
    ui->setupUi(this);

    this->toxic = toxic;
    this->pns = pns;
    this->sociability = sociability;
    this->popularity = popularity;
    this->username = username;
    this->health = 100;
    onShowCharacter();
    t_addcomment = new QTimer(this);
    connect(t_addcomment, SIGNAL(timeout()), this, SLOT(onAddComment()));
    t_addcomment->start((5000 + ((100 - sociability)*250))/speed);
    onUpdateHealth();

}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}
//отображение окна профиля
void ProfileWindow::onShowCharacter() {
    ui->pns->setText(QString::number(pns));
    ui->toxic->setText(QString::number(toxic));
    ui->sociability->setText(QString::number(sociability));
    ui->popularity->setText(QString::number(popularity));
    ui->profilename->setText(username);

    int value = QRandomGenerator::global()->bounded(50);
    QPixmap pixmap("C:/Games/qtprojects/pk2/fotoavatar/" + QString::number(value) + ".png");
    ui->image_label->setPixmap(pixmap);
    ui->image_label->setScaledContents(true);
    ui->image_label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored );
    mProfileHash.insert(username, pixmap); //вставляем ключ-значение в хэш таблицу
}
//функция формирует комментарий, который посылается в окно CommentsWindow
void ProfileWindow::onAddComment() {
    QList<CommentsWindow*> cw_list = this->parentWidget()->findChildren<CommentsWindow*>();
    int windowCount = cw_list.count();
    int good,bad,neit;
    int yes_no = QRandomGenerator::global()->bounded(100);
    if (windowCount > 0) {
        // Обращаемся к первому окну типа CommentWindow
        CommentsWindow* cw = cw_list.at(windowCount-1);
        QString title = cw->windowTitle();
        if (title == "Политика") {
            bad = 80;
            neit = 10;
        }
        else if (title == "Мемы") {
            bad = 10;
            neit = 10;
        }
        else if (title == "Политические мемы") {
            bad = 40;
            neit = 10;
        }
        else if (title == "Треш") {
            bad = 40;
            neit = 20;
        }
        int usersCount = cw->findChildren<QPushButton*>().size();
        if (usersCount == 0) {
            yes_no = QRandomGenerator::global()->bounded(100);
            if (yes_no <= bad) cw->onAddComment(this, "emot_angry", nullptr);
            else if (yes_no <= neit) cw->onAddComment(this, "emot_neit", nullptr);
            else cw->onAddComment(this, "emot_happy", nullptr);
        }
        else {
            int flag = 1;
            int i = QRandomGenerator::global()->bounded(1, usersCount+1);
            for (; flag && i < usersCount; i++) {
                QPushButton* username = nullptr;
                ProfileWindow* pw = nullptr;
                do {
                    username = cw->findChildren<QPushButton*>()[i];
                    pw = cw->mPushUserImage.value(username);
                    i++;
                } while ((pw->username == this->username && i < usersCount) || pw->health < 1);
                if (!(i < usersCount)) break;
                yes_no = QRandomGenerator::global()->bounded(100);
                if (yes_no <= pw->popularity) {
                    yes_no = QRandomGenerator::global()->bounded(100);
                    if (friends.contains(pw->username)) {
                        if (yes_no < toxic*bad/400) {
                            cw->onAddComment(this, "emot_grief", pw);
                            int index = friends.indexOf(pw->username);
                            this->friends.remove(index);
                            index = pw->friends.indexOf(this->username);
                            pw->friends.remove(index);
                            onUpdateList("list_friends", pw, 1);
                        }
                        else cw->onAddComment(this, "emot_happy", pw);
                        flag = 0;
                    }
                    else if (enemies.contains(pw->username)) {
                        if (yes_no <= toxic*bad/200) {
                            cw->onAddComment(this, "emot_osk", pw);
                            pw->health = pw->health - (100 - pw->pns);
                        }
                        else if (yes_no <= toxic*bad/100) {
                            cw->onAddComment(this, "emot_angry", pw);
                            pw->health = pw->health - ((100 - pw->pns)/2);
                        }
                        else cw->onAddComment(this,"emot_neit", pw);
                        flag = 0;
                        pw->onUpdateHealth();
                    }
                    else {
                        if (yes_no <= toxic*bad/200) cw->onAddComment(this, "emot_neit", pw);
                        else if (yes_no <= toxic*bad/100) {
                            cw->onAddComment(this, "emot_angry", pw);
                            this->enemies.append(pw->username);
                            onUpdateList("list_enemies", pw, 0);
                            pw->enemies.append(this->username);
                            pw->health = pw->health - ((100 - pw->pns/2));
                            pw->onUpdateHealth();
                        }
                        else {
                            cw->onAddComment(this, "emot_happy", pw);
                            this->friends.append(pw->username);
                            onUpdateList("list_friends", pw, 0);
                            pw->friends.append(this->username);
                        }
                        flag = 0;
                    }
                }
            }
            if (flag) {
                yes_no = QRandomGenerator::global()->bounded(100);
                if (yes_no <= toxic*bad/200) cw->onAddComment(this, "emot_angry", nullptr);
                else if (yes_no <= toxic*bad/100) cw->onAddComment(this, "emot_grief", nullptr);
                else cw->onAddComment(this, "emot_happy", nullptr);
            }
        }
    }
}
//функция изменения полоски состояния пользователя
void ProfileWindow::onUpdateHealth() {
    QProgressBar* health = this->findChild<QProgressBar*>("healthbar");
    QList<StatisticWindow*> sw_list = this->parentWidget()->findChildren<StatisticWindow*>();
    StatisticWindow* sw = sw_list.at(0);
    health->setValue(this->health);
    if (sw) sw->onUpdateStats(this, 0);
    if (this->health < 1) {
        health->setValue(0);
        if (sw) sw->onUpdateStats(this, 1);
        QList<CommentsWindow*> cw_list = this->parentWidget()->findChildren<CommentsWindow*>();
        int windowCount = cw_list.count();
        CommentsWindow* cw = cw_list.at(windowCount-1);
        int usersCount = cw->findChildren<QPushButton*>().size();
        cw->onAddComment(this, "emot_sad", nullptr);
        t_addcomment->stop();

        QPushButton* username = nullptr;
        ProfileWindow* pw = nullptr;
        int i = 0;
        do {
            username = cw->findChildren<QPushButton*>()[i];
            pw = cw->mPushUserImage.value(username);
            if (this->enemies.contains(pw->username)) {
                int index = pw->enemies.indexOf(this->username);
                pw->enemies.remove(index);
                pw->onMockeryComment(this, cw);
                index = enemies.indexOf(pw->username);
                enemies.remove(index);
            }
            else if (this->friends.contains(pw->username)) {
                int index = pw->friends.indexOf(this->username);
                pw->friends.remove(index);
                index = friends.indexOf(pw->username);
                friends.remove(index);
            }
            i++;   
        } while (i < usersCount);
    }
}
//функция создания комментария-реакции на лив соперника
void ProfileWindow::onMockeryComment(ProfileWindow* pw, CommentsWindow* cw) {
    int yes_no = QRandomGenerator::global()->bounded(100);
    int check_hp = this->health;
    if (yes_no <= this->toxic/2) {
        cw->onAddComment(this, "emot_swag", pw);
    }
    else
    {
        cw->onAddComment(this, "emot_mock", pw);
    }
   onUpdateHealth();
}
//функция обновления списка друзей\врагов
void ProfileWindow::onUpdateList(QString list_name, ProfileWindow* pw, bool isDelete) {
    if (!isDelete) {
        QListWidget *list = qobject_cast<QListWidget*>(this->findChild<QListWidget*>(list_name));
        list->insertItem(0, pw->username);
        if (pw != nullptr) {
            list = qobject_cast<QListWidget*>(pw->findChild<QListWidget*>(list_name));
            list->insertItem(0, this->username);
        }
    }
    else {
        QListWidget *list = qobject_cast<QListWidget*>(this->findChild<QListWidget*>(list_name));
        QListWidgetItem* item = list->findItems(pw->username, Qt::MatchExactly).first();
        if (item) {
            list->takeItem(list->row(item));
            delete item;
        }
        if (pw != nullptr) {
            list = qobject_cast<QListWidget*>(pw->findChild<QListWidget*>(list_name));
            item = list->findItems(this->username, Qt::MatchExactly).first();
            if (item) {
                list->takeItem(list->row(item));
                delete item;
            }
        }
    }
}
//изменить скорость добавления комментария согласно слайдеру в MainWindow
void ProfileWindow::onUpdateTimer() {
    t_addcomment->setInterval((5000 + ((100 - sociability)*250))/speed);
}
//поставить на паузу таймер(не писать комментарии)
void ProfileWindow::onPause() {
    t_addcomment->setInterval(100000000);
}


