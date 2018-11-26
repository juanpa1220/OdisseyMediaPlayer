#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qboxlayout.h>
#include <iostream>
#include "cliente.h"
#include "dialogmetadata.h"
using std::cout;

namespace Ui {
class Widget;
}

class QMediaPlayer;
class QVideoWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_open_clicked();

    void on_playpause_clicked();

    void on_rewind_clicked();

    void on_forward_clicked();

    void on_stop_clicked();

    void on_mute_clicked();

    void on_volume_valueChanged(int value);

    void on_search_clicked();

    void on_updateButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *mMediaPlayer;
    QVideoWidget *mVideoWidget;
    bool isMute;
    Cliente *cliente;
    std::string videoNameInput;
};

#endif // WIDGET_H
