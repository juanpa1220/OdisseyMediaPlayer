#include "dialogmetadata.h"
#include "ui_dialogmetadata.h"

DialogMetadata::DialogMetadata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMetadata)
{
    ui->setupUi(this);
}

DialogMetadata::~DialogMetadata()
{
    delete ui;
}

void DialogMetadata::on_okButton_clicked()
{
    this->setName(ui->name->text().toStdString());
    this->setTime(ui->time->text().toStdString());
    this->setArtist(ui->artist->text().toStdString());
    this->setAlbum(ui->album->text().toStdString());
    this->setGenre(ui->genre->text().toStdString());
    this->setYear(ui->year->text().toStdString());

    this->close();

    json respuesta;
    respuesta["Titulo: "] = getName();
    respuesta["Artista: "] = getArtist();
    respuesta["Album: "] = getAlbum();
    respuesta["Contenedor: "] = getGenre();
    respuesta["Año: "] = getYear();
    respuesta["Duracion: "] = getTime();

    std::string respuestaFinal = respuesta.dump();
    char *respuestaFinalFinal = &respuestaFinal[0u];

    int tamano = respuestaFinal.length();
    cliente->enviarDatos(std::to_string(tamano));
    cliente->enviarDatos(respuestaFinalFinal);

    std::cout << getName() << std::endl;
    std::cout << getTime() << std::endl;
    std::cout << getArtist() << std::endl;
    std::cout << getAlbum() << std::endl;
    std::cout << getGenre() << std::endl;
    std::cout << getYear() << std::endl;

}

std::string DialogMetadata::getYear() const
{
    return year;
}

void DialogMetadata::setYear(const std::string &value)
{
    year = value;
}

void DialogMetadata::setActualmetadata(const std::string value)
{
    actualmetadata = value;

    json json = json::parse(actualmetadata);

    std::string name = json["Titulo: "];
    ui->name->setText(QString::fromStdString(name));

    std::string artist = json["Artista: "];
    ui->artist->setText(QString::fromStdString(artist));

    std::string album = json["Album: "];
    ui->album->setText(QString::fromStdString(album));

    std::string genre = json["Contenedor: "];
    ui->genre->setText(QString::fromStdString(genre));

    std::string year = json["Año: "];
    ui->year->setText(QString::fromStdString(year));

    std::string time = json["Duracion: "];
    ui->time->setText(QString::fromStdString(time));
}

void DialogMetadata::setCliente(Cliente *value)
{
    cliente = value;
}

std::string DialogMetadata::getGenre() const
{
    return genre;
}

void DialogMetadata::setGenre(const std::string &value)
{
    genre = value;
}

std::string DialogMetadata::getAlbum() const
{
    return album;
}

void DialogMetadata::setAlbum(const std::string &value)
{
    album = value;
}

std::string DialogMetadata::getArtist() const
{
    return artist;
}

void DialogMetadata::setArtist(const std::string &value)
{
    artist = value;
}

std::string DialogMetadata::getTime() const
{
    return time;
}

void DialogMetadata::setTime(const std::string &value)
{
    time = value;
}

std::string DialogMetadata::getName() const
{
    return name;
}

void DialogMetadata::setName(const std::string &value)
{
    name = value;
}
