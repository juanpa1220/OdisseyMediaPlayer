#include "widget.h"
#include "ui_widget.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <qstandardpaths.h>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mMediaPlayer = new QMediaPlayer(this);
    mVideoWidget = new QVideoWidget(this);
    mMediaPlayer->setVideoOutput(mVideoWidget);
    ui->controlsLayout->insertWidget(0,mVideoWidget);
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
    cliente->detener();
    delete ui;
}

void Widget::on_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Abrir");
    std::cout<< filename.toStdString() << std::endl;
    if (filename.isEmpty()){
        return;
    }
    cliente->guardarVideo(filename.toStdString());
}


void Widget::on_playpause_clicked()
{
    switch (mMediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
//        std::string test = cliente->reproducirVideo();
//        QString filename = QString::fromStdString(test);
//        std::cout << filename.toStdString() << std::endl;
//        mMediaPlayer->setMedia(QUrl::fromLocalFile(filename));
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

void Widget::on_search_clicked()
{
    videoNameInput = ui->videoNameInput->text().toStdString() + + ".mp4";
    std::cout << videoNameInput << std::endl;

    bool respuesta = cliente->buscarVideo(videoNameInput);

    if (respuesta){
        std::cout<< "si encontro" << std::endl;
        QString filename = QString::fromStdString(cliente->reproducirVideo());
        mMediaPlayer->setMedia(QUrl::fromLocalFile(filename));
        mMediaPlayer->play();
    }else{
        std::cout<< "no encontro" << std::endl;
}
}

void Widget::on_updateButton_clicked()
{
    videoNameInput = ui->videoNameInput->text().toStdString() + ".mp4";
    std::cout << videoNameInput << std::endl;

    std::string actualMetadata = cliente->cambiarMetadata(videoNameInput);

    if (actualMetadata != "Noencontrado"){

        DialogMetadata *dialogMetadata = new DialogMetadata(this);
        dialogMetadata->setActualmetadata(actualMetadata);
        dialogMetadata->setCliente(cliente);


        dialogMetadata->setModal(true);
        dialogMetadata->show();
    }else{
        QMessageBox::information(this, tr("ERROR"), tr("EL VIDEO SOLICITADO NO FUE ENCONTRADO"));
    }
}

void Widget::on_deleteButton_clicked()
{
    videoNameInput = ui->videoNameInput->text().toStdString() + ".mp4";
    std::cout << videoNameInput << std::endl;

    if(!cliente->eliminarVideo(videoNameInput)){
        QMessageBox::information(this, tr("ERROR"), tr("EL VIDEO SOLICITADO NO FUE ENCONTRADO y POR ENDE NO SE PUDO ELIMINAR"));
    }else{
        std::string mensaje = "El video " + ui->videoNameInput->text().toStdString() + "se ha eliminado";
        QMessageBox::information(this, tr("ERROR"), tr(mensaje.c_str()));
    }

}
