#pragma once

#include <cmath>
#include <string>
#include <QIcon>
#include <QPushButton>

using namespace std; // Dans ce cours on accepte le using namespace std dans le .hpp .

class Piece {

public:
    //Piece() = default;
    Piece() = default;
    Piece(string nom = "", bool couleur = true) : nom_(nom), estBlanc_(couleur) {}
    ~Piece() = default;

    virtual bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) = 0;

    bool estBlanc_;
    string nom_ = "";
    string path_ = "";

    QIcon image;

};

class Roi : public Piece {
public:
    static int compteurRoi;

    Roi() : Piece("Roi", "images_pieces/Roiblanc16bit.png") { compteurRoi++; };
    Roi(bool estBlanc) : Piece("Roi",estBlanc) {
        if (estBlanc_)
            path_ = "images_pieces/Roiblanc16bit.png";
        else {
            path_ = "images_pieces/Roinoir16bit.png";
        }
        QIcon icon(QString::fromStdString(path_));
        image = icon;
        compteurRoi++;
    };
    ~Roi() = default;

    bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) override;
};

class Reine : public Piece {
public:

    Reine() : Piece("Reine", "images_pieces/Reineblanc16bit.png") {};
    Reine(bool estBlanc) : Piece("Reine", estBlanc) {
        if (estBlanc_)
            path_ = "images_pieces/Reineblanc16bit.png";
        else {
            path_ = "images_pieces/Reinenoir16bit.png";
        }
        QIcon icon(QString::fromStdString(path_));
        image = icon;
    };
    ~Reine() = default;

    bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) override;
};
class Cavalier : public Piece {
public:

    Cavalier() : Piece("Cavalier", "images_pieces/Cavalierblanc16bit.png") {};
    Cavalier(bool estBlanc) : Piece("Cavalier",estBlanc) {
        if (estBlanc_)
            path_ = "images_pieces/Cavalierblanc16bit.png";
        else {
            path_ = "images_pieces/Cavaliernoir16bit.png";
        }
        QIcon icon(QString::fromStdString(path_));
        image = icon;
    };
    ~Cavalier() = default;

    bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) override;
};

class Pion : public Piece {
public:
    Pion() : Piece("Pion", "images_pieces/Pionblanc16bit.png") {};

    Pion(bool estBlanc) : Piece("Pion", estBlanc) {
        if (estBlanc_)
            path_ = "images_pieces/Pionblanc16bit.png";
        else {
            path_ = "images_pieces/Pionnoir16bit.png";
        }
        QIcon icon(QString::fromStdString(path_));
        image = icon;
    };
    ~Pion() = default;

    bool premier_mouv = true;

    bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) override;
};


class Tour : public Piece {
public:
    Tour() : Piece("Tour", "images_pieces/Tourblanc16bit.png") {};

    Tour(bool estBlanc) : Piece("Tour",estBlanc) {
        if (estBlanc_)
            path_ = "images_pieces/Tourblanc16bit.png";
        else {
            path_ = "images_pieces/Tournoir16bit.png";
        }
        QIcon icon(QString::fromStdString(path_));
        image = icon;
    };
    ~Tour() = default;


    bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) override;
};

class Fou : public Piece {
public:
    Fou() : Piece("Fou", "images_pieces/Foublanc16bit.png") {};

    Fou(bool estBlanc) : Piece("Fou",estBlanc) {
        if (estBlanc_)
            path_ = "images_pieces/Foublanc16bit.png";
        else {
            path_ = "images_pieces/Founoir16bit.png";
        }
        QIcon icon(QString::fromStdString(path_));
        image = icon;
    };
    ~Fou() = default;

    
    bool estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) override;
};
