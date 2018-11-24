#include "widget.h"
#include "ui_widget.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <qstandardpaths.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mMediaPlayer = new QMediaPlayer(this);
    mVideoWidget = new QVideoWidget(this);
    mMediaPlayer->setVideoOutput(mVideoWidget);
//    auto L = static_cast<QBoxLayout *>(layout());
//    L->insertWidget(0, mVideoWidget);
    ui->controlsLayout->insertWidget(0,mVideoWidget);

//    mMediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
//    mVideoWidget = new QVideoWidget;
//    mMediaPlayer->setVideoOutput(mVideoWidget);
//    mLayout = new QVBoxLayout;
//    mLayout->addWidget(mVideoWidget);

    isMute = false;

    connect(mMediaPlayer, &QMediaPlayer::positionChanged, [&](int pos){
        ui->progressBar->setValue(pos);
    });
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, [&](int dur){
        ui->progressBar->setMaximum(dur);
    });


    // inicializa el cliente
    cliente = new Cliente(sockaddr_in());
    cliente->iniciar();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Abrir");
    std::cout<< filename.toStdString() << std::endl;
    if (filename.isEmpty()){
        return;
    }
    cliente->subirVideo(filename.toStdString());

//    on_playpause_clicked();
}


void Widget::on_playpause_clicked()
{
    switch (mMediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
        QString filename = QString::fromStdString(cliente->recibirVideo());
        mMediaPlayer->setMedia(QUrl::fromLocalFile(filename));
        mMediaPlayer->play();
        break;
    }
}

void Widget::on_rewind_clicked()
{
    mMediaPlayer->setPosition(ui->progressBar->value() - (ui->progressBar->maximum() / 10));
}

void Widget::on_forward_clicked()
{
    mMediaPlayer->setPosition(mMediaPlayer->position() + (ui->progressBar->maximum() / 10));
}

void Widget::on_stop_clicked()
{
    mMediaPlayer->stop();
    mMediaPlayer->setPosition(0);
    on_playpause_clicked();
}

void Widget::on_mute_clicked()
{
    if (isMute){
            mMediaPlayer->setMuted(false);
            ui->mute->setText("🔇");
            isMute = false;
    }else{
        mMediaPlayer->setMuted(true);
        ui->mute->setText("🔈");
        isMute = true;
    }
}

void Widget::on_volume_valueChanged(int value)
{
    mMediaPlayer->setVolume(value);
}
