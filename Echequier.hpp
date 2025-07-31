#pragma once

#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include "Piece.hpp"
#include <array>
#include <QIcon>
#include <stdexcept>
#include <QDialog>
#include <QLineEdit>

#include <QLabel>
#include <QMessageBox>
#include <QString>

#include <QPixmap>
#include <iostream>

enum class ChoixPartie{Classique, Perso, Fin};

struct Coordonnee { int x, y; };

class Case : public QPushButton {
public:
    Case(int x, int y) : coordonnee{ x, y } {
        QIcon icon(QString::fromStdString(""));
        setIcon(icon);
        setFixedSize(75, 75);
        setIconSize(QSize(50, 50));
    };
    Case() {
        QIcon icon(QString::fromStdString(""));
        setIcon(icon);
        setFixedSize(75, 75);
        setIconSize(QSize(50, 50));
    };
    Case(Piece* piece) {
        QIcon icon(QString::fromStdString(""));
        setIcon(icon);
        setFixedSize(75, 75);
        setIconSize(QSize(50, 50));
        element = piece;
    };
    Case(const Case& a) : element(a.element), couvertBlanc(a.couvertBlanc), couvertNoir(a.couvertNoir), coordonnee(a.coordonnee) {};

    Piece* element = nullptr;

    bool couvertBlanc = false;
    bool couvertNoir = false;

    bool estClique = false;

    Coordonnee coordonnee;
};

class Echequier {
public:
    Echequier();
    ~Echequier() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (table[i][j]->element != nullptr)
                    delete table[i][j]->element;

    };
    bool echecetmat(Case*& caseRoi);

    bool estcapturee(bool capture);

    void setPiece(Piece* piece, int positionX, int positionY) {
        table[positionX][positionY]->element = piece;

        Roi* roi = dynamic_cast<Roi*>(piece);

        if (roi != nullptr)
            if (roi->compteurRoi > 2) {
                roi->compteurRoi--;

                throw runtime_error(to_string(positionX) + to_string(positionY));
            }
    }


    void pieceTemporaire(Piece* piece, int positionX, int positionY);

    void deplacement();

    bool condition_deplacement(Case* premier, Case* deuxieme);

    bool cheminLibre(Case* premier, Case* deuxieme);

    void ajouter(Case* nouv_case);

    bool pionCapture(Case* caseActuel, Case* caseFutur);

    void effacer_caseSelect();

    void verifier_pion();

    void couverture();

    void colorier_couverture(Case* case_piece, Case*& case_colorier);

    void colorier_CouverturePion(Case* case_pion);

    void Roi_echec();

    void setPieceDefaut();

    bool VerifierPieceRoi(Case* case1);

    bool VerifierPiecePion(Case* case1);

    bool VerifierPieceCavalier(Case* case1);

    void choixDeplacement(Case* case_choix);

    array<Case*, 2> caseSelect;
    int nbElement_caseSelect = 0;


    bool estTourBlanc = true;
    bool finDePartie = false;
    Coordonnee positionRoiBlanc;
    Coordonnee positionRoiNoir;

    Case* caseClique = nullptr;
    array<array<Case*, 8>, 8> table;
};


class EchequierWindow : public QMainWindow {

public:
    bool estClique = false;

    EchequierWindow(QWidget* parent = nullptr);
    ~EchequierWindow() = default;

private:
    int creerFentrePerso(bool estBlanc, int nPiecesBlanches);
    pair<int, int> creerFentrePerso2Pos(int nPiece);
    void creerFentrePerso2Piece(pair<int, int> pair, bool estBlanc);
    Echequier echequier;
};

class MenuFinDePrtie: public QMainWindow {
public:






private:





};