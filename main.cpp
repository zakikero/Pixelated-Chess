#include "Echequier.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    EchequierWindow fenetre;

    fenetre.show();

    return app.exec();
}