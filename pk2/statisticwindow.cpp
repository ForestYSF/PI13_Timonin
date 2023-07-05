#include "statisticwindow.h"
#include "ui_statisticwindow.h"

StatisticWindow::StatisticWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticWindow)
{
    ui->setupUi(this);
}

StatisticWindow::~StatisticWindow()
{
    delete ui;
}
//обновление статус-бара пользователя\добавление информации о новом пользователе
void StatisticWindow::onUpdateStats(ProfileWindow* pw, bool isOff) {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->vlayout->layout()); //указатель на вертикальный компоновщик
    QHBoxLayout* newLayout = new QHBoxLayout(ui->vlayout->parentWidget());  //указать на горизонтальный компоновщик
    if (users.contains(pw->username) && !isOff) {
        QProgressBar *progressBar = this->findChild<QProgressBar*>(pw->username);
        progressBar->setValue(pw->health);
    }
    else if (users.contains(pw->username) && isOff) {
        QProgressBar *progressBar = this->findChild<QProgressBar*>(pw->username);
        progressBar->setValue(0);
    }
    else {
        users.append(pw->username);

        QPixmap image_pixmap = pw->mProfileHash.value(pw->username);
        QLabel* image = new QLabel("",ui->vlayout->parentWidget());
        QPixmap scaled_image = image_pixmap.scaled(70,70, Qt::KeepAspectRatio);
        image->setPixmap(scaled_image);

        QProgressBar *progressBar = new QProgressBar(this);
        progressBar->setObjectName(pw->username);
        progressBar->setValue(pw->health);

        QPushButton* username = new QPushButton(pw->username + ": ",ui->vlayout->parentWidget());
        username->setFixedSize(100,30);
        QSize size_image(30,30);
        image->setMinimumSize(size_image);
        image->setMaximumSize(size_image);

        newLayout->addWidget(image);
        newLayout->addWidget(username);
        newLayout->addWidget(progressBar);

        layout->addLayout(newLayout);
        mOpenUserProfile.insert(username, pw);
        connect(username, SIGNAL(clicked()), this, SLOT(on_ShowUser()));
    }

}
//открыть профиль пользователя
void StatisticWindow::on_ShowUser() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    ProfileWindow* pw = mOpenUserProfile.value(button);
    pw->show();
}
