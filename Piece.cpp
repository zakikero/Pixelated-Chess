#include "Piece.hpp"


int Roi::compteurRoi = 0;


bool Roi::estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) {

	using namespace std;

	if ((abs(xProchain - xActuel) <= 1) && (abs(yProchain - yActuel) <= 1))
		return true;

	return false;
};

bool Reine::estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) {

	using namespace std;

	bool condition_Tour = (abs(xProchain - xActuel) > 0) ^ (abs(yProchain - yActuel) > 0);
	bool condition_fou = (abs(xProchain - xActuel) == abs(yProchain - yActuel) && abs(xProchain - xActuel) > 0);
	if (condition_fou || condition_Tour)
		return true;

	return false;
};

bool Cavalier::estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) {

	using namespace std;

	bool condition_cavalier = (abs(xProchain - xActuel) == 2 && abs(yProchain - yActuel) == 1) ^ 
							  (abs(xProchain - xActuel) == 1 && abs(yProchain - yActuel) == 2);

	if (condition_cavalier)
		return true;

	return false;
};



bool Pion::estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) {

	switch (estBlanc_) {
	case false:
		switch (premier_mouv) {
			case true:
				if (((xProchain - xActuel) > 0 && (xProchain - xActuel) <= 2 && (yProchain - yActuel) == 0)) {
					return true;
				}
			case false:
				if (((xProchain - xActuel) > 0 && (xProchain - xActuel) <= 1 && (yProchain - yActuel) == 0))
					return true;
		}

	case true:

		switch (premier_mouv) {
			case true:
				if (((xProchain - xActuel) < 0 && (xProchain - xActuel) >= -2 && (yProchain - yActuel) == 0)) {
					return true;
				}
			case false:
				if ((xProchain - xActuel) < 0 && (xProchain - xActuel) >= -1 && (yProchain - yActuel) == 0)
					return true;
		}
	}

	return false;
}


bool Tour::estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) {

	using namespace std;

	if ((abs(xProchain - xActuel) > 0) ^ (abs(yProchain - yActuel) > 0))
		return true;

	return false;
};

bool Fou::estValideMouvement(int xActuel, int yActuel, int xProchain, int yProchain) {

	using namespace std;

	if (abs(xProchain - xActuel) == abs(yProchain - yActuel) && abs(xProchain - xActuel)>0)
		return true;

	return false;
};
// faire condition avec blocage de chemin pour tour et fou