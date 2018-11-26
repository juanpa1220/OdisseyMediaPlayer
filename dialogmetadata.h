#ifndef DIALOGMETADATA_H
#define DIALOGMETADATA_H
#include <iostream>
#include <QDialog>
#include "cliente.h"
#include "JSON/json.hpp"
using json = nlohmann::json;

namespace Ui {
class DialogMetadata;
}

class DialogMetadata : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMetadata(QWidget *parent = nullptr);
    ~DialogMetadata();

    std::string getName() const;
    void setName(const std::string &value);

    std::string getTime() const;
    void setTime(const std::string &value);

    std::string getArtist() const;
    void setArtist(const std::string &value);

    std::string getAlbum() const;
    void setAlbum(const std::string &value);

    std::string getGenre() const;
    void setGenre(const std::string &value);

    std::string getYear() const;
    void setYear(const std::string &value);

    void setCliente(Cliente *value);

    void setActualmetadata(const std::string value);

    Cliente *cliente;
    std::string actualmetadata;

private slots:
    void on_okButton_clicked();

private:
    Ui::DialogMetadata *ui;

    std::string name;
    std::string time;
    std::string artist;
    std::string album;
    std::string genre;
    std::string year;
};

#endif // DIALOGMETADATA_H
