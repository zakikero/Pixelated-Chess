#include "Echequier.hpp"

void afficherMessage(string message) {
	QMessageBox::information(nullptr, "Message", QString::fromStdString(message));
}

void Echequier::deplacement() {

	estTourBlanc = !estTourBlanc;

	Piece* tampon = move(caseSelect[0]->element);

	caseSelect[0]->element = move(caseSelect[1]->element);

	caseSelect[1]->element = move(tampon);



	if (caseSelect[0]->element != nullptr) {
		caseSelect[0]->setIcon(caseSelect[0]->element->image);
	}
	else {
		QIcon icon(QString::fromStdString(""));
		caseSelect[0]->setIcon(icon);
	}
	caseSelect[1]->element = nullptr;
	QIcon icon(QString::fromStdString(""));
	caseSelect[1]->setIcon(icon);

	delete tampon;
}
// quelques bug de deplacement 

bool Echequier::cheminLibre(Case* case1, Case* case2) {

	//permet de ne pas pouvoir manger les pieces alliees + couvre les peices entre elles
	if (case2->element != nullptr)
		if (case1->element->estBlanc_ == case2->element->estBlanc_) {
			if (case1->element->estBlanc_) {
				case2->couvertBlanc = true;
			}
			else { 
				case2->couvertNoir = true;
			}
			return false;
		}
	

	//verifie les cases non couvert pour roi
	Roi* roi = dynamic_cast<Roi*>(case1->element);
	if (VerifierPieceRoi(case1)) {
		switch (roi->estBlanc_) {
		case true:
			if (roi->estBlanc_ == case2->couvertNoir)
				return false;
		case false:
			if (!roi->estBlanc_ == case2->couvertBlanc)
				return false;
		}
	}

	if (VerifierPieceCavalier(case1))
		return true;

	if (VerifierPiecePion(case1) && case2->element != nullptr)
		return false;

	if (case2->coordonnee.x - case1->coordonnee.x == 0 && case2->coordonnee.y - case1->coordonnee.y==0)
		return false;
	int deplacement_x = case2->coordonnee.x - case1->coordonnee.x;
	int deplacement_y = case2->coordonnee.y - case1->coordonnee.y;

	if (case2->coordonnee.x - case1->coordonnee.x != 0)
		deplacement_x=deplacement_x/abs(case2->coordonnee.x - case1->coordonnee.x);

	if (case2->coordonnee.y - case1->coordonnee.y != 0)
		deplacement_y = deplacement_y / abs(case2->coordonnee.y - case1->coordonnee.y);


	//std::cout << "creation de delta x et y" << endl;
	//std::cout << "x : " << deplacement_x << " y : " << deplacement_y << endl;

	

	for (int i = case1->coordonnee.x + deplacement_x, j = case1->coordonnee.y + deplacement_y; i != case2->coordonnee.x || j != case2->coordonnee.y; i += deplacement_x, j += deplacement_y)
		if (table[i][j]->element != nullptr) {

			//std::cout << "obstruction trouvee retourne false" << endl;
		return false;
		}
	

	return true;
};

bool Echequier::pionCapture(Case* caseActuel, Case* caseFutur) {


	if (caseFutur->element != nullptr && caseFutur->element->estBlanc_ != caseActuel->element->estBlanc_) {	
		switch (caseActuel->element->estBlanc_) {
		case true:
			if (caseFutur->coordonnee.x - caseActuel->coordonnee.x == -1 && abs(caseFutur->coordonnee.y - caseActuel->coordonnee.y) == 1)
				return true;
		case false:
			if ((caseFutur->coordonnee.x - caseActuel->coordonnee.x) == 1 && abs(caseFutur->coordonnee.y - caseActuel->coordonnee.y) == 1)
				return true;
		}
	}
	return false;
}

bool Echequier::condition_deplacement(Case* case1, Case* case2) {

	if (VerifierPiecePion(case1) && pionCapture(case1, case2))
		return true;


	if (case1->element!=nullptr )
		if (case1->element->estValideMouvement(caseSelect[1]->coordonnee.x, caseSelect[1]->coordonnee.y, caseSelect[0]->coordonnee.x, caseSelect[0]->coordonnee.y)
			&& cheminLibre(case1, case2)) {

			if (VerifierPieceRoi(caseSelect[0])) {
				if (caseSelect[0]->element->estBlanc_) {
					afficherMessage("Le Roi blanc est mort, les Noir sont vainqueurs fin de partie ");
				}
				else {
					afficherMessage("Le Roi Noir est mort, les blanc sont vainqueurs fin de partie ");
				}
				finDePartie = true;
			}
			return true;
			
		}
		
	return false;
}


void Echequier::ajouter(Case* nouv_case) {
	caseSelect[1] = move(caseSelect[0]);
	caseSelect[0] = move(nouv_case);
	nbElement_caseSelect++;
}

void Echequier::effacer_caseSelect() {

	//std::cout << "effage de caseSelect" << endl;
	caseSelect[0] = nullptr;
	caseSelect[1] = nullptr;
	//std::cout << "etat de caseSelect : " << caseSelect[0] << " " << caseSelect[1] << endl;
	nbElement_caseSelect = 0;
}

void Echequier::verifier_pion() {

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j += 7) {
			if (table[j][i]->element !=nullptr && table[j][i]->element->nom_ == "Pion") {
				std::cout << endl << j << " " << i << endl;
				//creer interface changement piece
				std::cout << "pion a atteint le bout et est pret a changer" << endl;
				std::cout << "entrez la nouvelle piece voulu";
				bool couleur = table[j][i]->element->estBlanc_;

				QDialog* newDialog = new QDialog();
				newDialog->setWindowTitle("Choississz le remplacement");
				newDialog->resize(300, 100);
				newDialog->setModal(true);

				QGridLayout* layoutSecondaire = new QGridLayout();

				//boutton piece 
				QPushButton* tour = new QPushButton();
				tour->setText(QString::fromStdString("Tour"));
				QObject::connect(tour, &QPushButton::clicked, [&]() {
					table[j][i]->element = new Tour(couleur);
					table[j][i]->setIcon(table[j][i]->element->image);
					newDialog->close();
					});
				layoutSecondaire->addWidget(tour);

				//boutton piece Fou
				QPushButton* fou = new QPushButton();
				fou->setText(QString::fromStdString("Fou"));
				QObject::connect(fou, &QPushButton::clicked, [&]() {
					table[j][i]->element = new Fou(couleur);
					table[j][i]->setIcon(table[j][i]->element->image);
					newDialog->close();
					});
				layoutSecondaire->addWidget(fou);

				//boutton piece Cavalier
				QPushButton* cavalier = new QPushButton();
				cavalier->setText(QString::fromStdString("Cavalier"));
				QObject::connect(cavalier, &QPushButton::clicked, [&]() {
					table[j][i]->element = new Cavalier(couleur);
					table[j][i]->setIcon(table[j][i]->element->image);
					newDialog->close();
					});
				layoutSecondaire->addWidget(cavalier);

				//boutton piece Reine
				QPushButton* reine = new QPushButton();
				reine->setText(QString::fromStdString("Reine"));
				QObject::connect(reine, &QPushButton::clicked, [&]() {
					table[j][i]->element = new Reine(couleur);
					table[j][i]->setIcon(table[j][i]->element->image);
					newDialog->close();
					});
				layoutSecondaire->addWidget(reine);

				newDialog->setLayout(layoutSecondaire);

				newDialog->show();
				newDialog->exec();
			}
		}
}

void Echequier::colorier_couverture(Case* case_piece,Case*& case_colorier) {


	if (case_piece->element->estBlanc_) {
		/*case_colorier->setStyleSheet("QPushButton { border:none;padding: 5px;background-color: rgb(0,152,193);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
			"QPushButton:pressed {background-color: red;}");*/
		case_colorier->couvertBlanc = true;
	}
	else {
		/*case_colorier->setStyleSheet("QPushButton { border:none;padding: 5px;background-color: rgb(206,0,0);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
			"QPushButton:pressed {background-color: red;}");*/
		case_colorier->couvertNoir = true;
	}
	//si couvert par noir et blanc
	/*if (case_colorier->couvertNoir && case_colorier->couvertBlanc);
		case_colorier->setStyleSheet("QPushButton {border:none;padding: 5px;background-color: rgb(132,0,231);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
		"QPushButton:pressed {background-color: red;}");*/
}

void Echequier::colorier_CouverturePion(Case* case_pion) {

	int pion_x = case_pion->coordonnee.x;
	int pion_y = case_pion->coordonnee.y;

	if ((pion_x - 1 < 0 || pion_x - 1 > 7) || (pion_y - 1 < 0 || pion_y - 1 > 7) || (pion_y + 1 < 0 || pion_y + 1 > 7))
		return;

	switch (case_pion->element->estBlanc_){
	case true:	
		colorier_couverture(case_pion, table[pion_x - 1][pion_y + 1]);
		colorier_couverture(case_pion, table[pion_x - 1][pion_y - 1]);
		return;

	case false:
		colorier_couverture(case_pion, table[pion_x + 1][pion_y + 1]);
		colorier_couverture(case_pion, table[pion_x + 1][pion_y - 1]);
		return;
	}
}


void Echequier::couverture() {

	std::cout << "couverture en cours" << endl;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (table[i][j]->couvertBlanc || table[i][j]->couvertNoir) {
				//std::cout << "reset" << endl;
				table[i][j]->couvertBlanc = false;
				table[i][j]->couvertNoir = false;
				if ((i + j) % 2 == 0) {
					table[i][j]->setStyleSheet("QPushButton { border:none;padding: 5px;background-color: rgb(215,218,220);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
						"QPushButton:pressed {background-color: red;}");
				}
				else {
					table[i][j]->setStyleSheet("QPushButton {border:none;padding: 5px;background-color: rgb(133,133,133);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
						"QPushButton:pressed {background-color: red;}");
				}
			}

	for(int i=0;i<8;i++)
		for (int j = 0; j < 8; j++) {
			Case* case_piece = table[i][j];
			//test de coordonnee
			for (int x = 0; x < 8; x++)
				for (int y = 0; y < 8; y++) {
					Case* case_test = table[x][y];

					//couverture de pion
					if (!VerifierPiecePion(case_piece) && case_piece->element != nullptr &&
						table[i][j]->element->estValideMouvement(case_piece->coordonnee.x, case_piece->coordonnee.y, case_test->coordonnee.x, case_test->coordonnee.y)
							&& cheminLibre(case_piece, case_test)) {

								//std::cout << "couleur" << endl;
							colorier_couverture(table[i][j], table[x][y]);

					}
					else if(VerifierPiecePion(case_piece)){
							colorier_CouverturePion(table[i][j]);
					}	

				}
		}
	std::cout << "fin couverture" << endl;
}

bool Echequier::echecetmat(Case*& caseRoi) {

	int position_x = caseRoi->coordonnee.x;
	int position_y = caseRoi->coordonnee.y;

	int x, y;
	
	//verifie les case autour du roi pour voir si deplacement pourrais se faire
	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++) {
			
			x = position_x + i;
			y = position_y + j;

			/*if (!(x < 0 || x > 7) && !(y < 0 || y > 7) && !(x == position_x && y == position_y))
				cout << "coord. accepte ";*/
			
			//assert(((x < 0 || x > 7) || (y < 0 || y > 7)));
			
			if (!(x < 0 || x > 7) && !(y < 0 || y > 7) && !(x == position_x && y == position_y)
				&& caseRoi->element->estValideMouvement(caseRoi->coordonnee.x, caseRoi->coordonnee.y,table[x][y]->coordonnee.x, table[x][y]->coordonnee.y)
				&& cheminLibre(caseRoi,table[x][y])) {
				table[x][y]->setStyleSheet("QPushButton { border:none;padding: 5px;background-color: rgb(190,0,0);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
				"QPushButton:pressed {background-color: red;}");
				std::cout <<"x : " << x << " y : " << y << endl;
				return false;
			}
		}
	return true;
}

void Echequier::Roi_echec() {

	/*std::cout<<"couvertNoir : "<< boolalpha << table[positionRoiBlanc.x][positionRoiBlanc.y]->couvertNoir << endl;
	std::cout <<"couvertBlanc : "<< boolalpha << table[positionRoiNoir.x][positionRoiNoir.y]->couvertBlanc << endl;*/

	if (table[positionRoiBlanc.x][positionRoiBlanc.y]->couvertNoir) {
		
		if (echecetmat(table[positionRoiBlanc.x][positionRoiBlanc.y])) {
			afficherMessage("Le Roi blanc est en echec et mat, les Noirs sont vainqueurs");
			finDePartie = true;
		}
		else { afficherMessage("Le Roi blanc est en echec"); }	
	}
	else if (table[positionRoiNoir.x][positionRoiNoir.y]->couvertBlanc) {

		if (echecetmat(table[positionRoiNoir.x][positionRoiNoir.y])) {
			afficherMessage("Le Roi Noir est en echec et mat, les Blancs sont vainqueurs");
			finDePartie = true;
		}
		else{afficherMessage("Le Roi Noir est en echec");}
	}

}


bool Echequier::VerifierPieceRoi(Case* case1) {
	Roi* roi = dynamic_cast<Roi*>(case1->element);

	return roi != nullptr;
}

bool Echequier::VerifierPiecePion(Case* case1) {
	Pion* pion = dynamic_cast<Pion*>(case1->element);

	return pion != nullptr;
}

bool Echequier::VerifierPieceCavalier(Case* case1) {
	Cavalier* cavalier = dynamic_cast<Cavalier*>(case1->element);

	return cavalier != nullptr;
}


EchequierWindow::EchequierWindow(QWidget* parent) : QMainWindow(parent) {

	QWidget* plateau = new QWidget(this);
	QGridLayout* layoutPrincipale = new QGridLayout();
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//connections du boutton
			connect(echequier.table[i][j], &QPushButton::clicked, [=]() {

				if ((echequier.table[i][j]->element != nullptr || echequier.caseSelect[0] != nullptr)) {
				//	std::cout << "case ajoutee" << endl;
					//echequier.choixDeplacement(echequier.table[i][j]);
					echequier.ajouter(echequier.table[i][j]);
				}
				if (echequier.nbElement_caseSelect == 2 ){

					if (echequier.caseSelect[1]->element->estBlanc_ == echequier.estTourBlanc
						&& echequier.condition_deplacement(echequier.caseSelect[1], echequier.caseSelect[0])) {
						echequier.deplacement();
						
						
						//Roi* roi = dynamic_cast<Roi*>(echequier.caseSelect[0]->element);

						if (echequier.VerifierPieceRoi(echequier.caseSelect[0])) {
							if (echequier.caseSelect[0]->element->estBlanc_)
								echequier.positionRoiBlanc = { echequier.caseSelect[0]->coordonnee.x,echequier.caseSelect[0]->coordonnee.y };
							else
								echequier.positionRoiNoir = { echequier.caseSelect[0]->coordonnee.x,echequier.caseSelect[0]->coordonnee.y };
						}

						Pion* pion = dynamic_cast<Pion*>(echequier.caseSelect[0] ->element);
						if (echequier.VerifierPiecePion(echequier.caseSelect[0]))
							pion->premier_mouv = false;
						
						echequier.verifier_pion();

						echequier.couverture();

						echequier.Roi_echec();


						if (echequier.finDePartie) {



							this->close();
						}
					}
					else {
						afficherMessage("deplacement non valide");
					}
					echequier.effacer_caseSelect();
				}
				});

			//ajout bu boutton
			layoutPrincipale->addWidget(echequier.table[i][j], i, j);

		}
	}

	//style de l'echequier
	layoutPrincipale->setSpacing(0);
	plateau->setLayout(layoutPrincipale);
	setCentralWidget(plateau);

}

Echequier::Echequier() {

	//atribution des valeurs de coordonnees (coter des blancs vers le bas comme chess.com)
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			//creation du boutton 
			table[i][j] = new Case(i,j);

			//atribution de la couleur du boutton
			if ((i + j) % 2 == 0) {
				table[i][j]->setStyleSheet("QPushButton { border:none;padding: 5px;background-color: rgb(215,218,220);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
						"QPushButton:pressed {background-color: red;}");
			}
			else {
				table[i][j]->setStyleSheet("QPushButton {border:none;padding: 5px;background-color: rgb(133,133,133);border-color:black;border-top-width: 500px; border-right-width: 500px; border-bottom-width: 500px;border-left-width: 500px; font-size: 100px;}"
						"QPushButton:pressed {background-color: red;}");
			}
		}
	Roi* roi = new Roi(false);
	setPiece(roi, 0, 4);
	positionRoiNoir = { 0,4 };

	Roi* roi2 = new Roi(true);
	setPiece(roi2, 7, 4);
	positionRoiBlanc = { 7,4 };

	for (int i = 0; i < 8; i++) {
		Pion* pionBlanc = new Pion(true);
		setPiece(pionBlanc, 6, i);
	}

	for (int i = 0; i < 8; i++) {
		Pion* pionNoir = new Pion(false);
		setPiece(pionNoir, 1, i);
	}

	Reine* reineNoir = new Reine(false);
	setPiece(reineNoir, 0, 3);

	Reine* reineBlanc = new Reine(true);
	setPiece(reineBlanc, 7, 3);



	Fou* fouNoir1 = new Fou(false);
	setPiece(fouNoir1, 0, 2);

	Fou* fouNoir2 = new Fou(false);
	setPiece(fouNoir2, 0, 5);

	Fou* fouBlanc1 = new Fou(true);
	setPiece(fouBlanc1, 7, 2);

	Fou* fouBlanc2 = new Fou(true);
	setPiece(fouBlanc2, 7, 5);


	Cavalier* cavalierNoir1 = new Cavalier(false);
	setPiece(cavalierNoir1, 0, 1);

	Cavalier* cavalierNoir2 = new Cavalier(false);
	setPiece(cavalierNoir2, 0, 6);

	Cavalier* cavalierBlanc1 = new Cavalier(true);
	setPiece(cavalierBlanc1, 7, 1);

	Cavalier* cavalierBlanc2 = new Cavalier(true);
	setPiece(cavalierBlanc2, 7, 6);


	Tour* tourNoir1 = new Tour(false);
	setPiece(tourNoir1, 0, 0);

	Tour* tourNoir2 = new Tour(false);
	setPiece(tourNoir2, 0, 7);

	Tour* tourBlanc1 = new Tour(true);
	setPiece(tourBlanc1, 7, 0);

	Tour* tourBlanc2 = new Tour(true);
	setPiece(tourBlanc2, 7, 7);

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (table[i][j]->element != nullptr) 
					table[i][j]->setIcon(table[i][j]->element->image);		
}