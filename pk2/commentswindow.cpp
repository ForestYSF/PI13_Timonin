#include "commentswindow.h"
#include "ui_commentswindow.h"
CommentsWindow::CommentsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommentsWindow)
{
    ui->setupUi(this);
}

CommentsWindow::~CommentsWindow()
{
    delete ui;
}
//возвращает количество комментариев под постом
int CommentsWindow::onCheckNewsCount() {
    QObjectList widgetList = ui->vlayout->children();
    return widgetList.count();
}
//добавление комментария под постом(формируется посланное сообщение из ProfileWindow и добавляется в окно)
void CommentsWindow::onAddComment(ProfileWindow* pw1, QString usermessage, ProfileWindow* pw2) {
    emit _addComment();


    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->vlayout->layout()); //указатель на вертикальный компоновщик
    QHBoxLayout* newLayout = new QHBoxLayout(ui->vlayout->parentWidget());  //указать на горизонтальный компоновщик
    QPixmap image_pixmap = pw1->mProfileHash.value(pw1->username);
    QLabel* image = new QLabel("",ui->vlayout->parentWidget());
    QPixmap scaled_image = image_pixmap.scaled(70,70, Qt::KeepAspectRatio);
    image->setPixmap(scaled_image);

    QLabel* comment = new QLabel("",ui->vlayout->parentWidget());
    QPixmap emot_image("C:/Games/qtprojects/pk2/fotoemot/" + usermessage + ".png");
    QPixmap scaled_emot = emot_image.scaled(50, 50, Qt::KeepAspectRatio);
    comment->setPixmap(scaled_emot);

    QPushButton* username = new QPushButton(pw1->username + ": ",ui->vlayout->parentWidget());
    username->setFixedSize(100,30);
    QSize size_image(30,30);
    image->setMinimumSize(size_image);
    image->setMaximumSize(size_image);

    newLayout->addWidget(image);
    newLayout->addWidget(username);
    newLayout->addWidget(comment);
    if (pw2 != nullptr) {
        QString text_comment = " (to " + pw2->username + ")";
        QLabel* repost = new QLabel(text_comment,ui->vlayout->parentWidget());
        repost->setText(text_comment);
        newLayout->addWidget(repost);
    }
    layout->addLayout(newLayout);


    mPushUserImage.insert(username, pw1);
    connect(username, SIGNAL(clicked()), this, SLOT(on_ShowUser()));

}
//открыть профиль пользователя
void CommentsWindow::on_ShowUser() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    ProfileWindow* pw = mPushUserImage.value(button);
    pw->show();
}

