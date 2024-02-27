#ifndef IA_H_
#define IA_H_
#include "jeu.h"

struct position {
    plateau* plateau;
    int joueur;
    int nb_pierres_j1;
    int nb_pierres_j2;
};
typedef struct position position;

bool terminale(position* pos);
position* initialiser_position(plateau* p, int j);
int jouer_coup(position* p, int puit);
void afficher_position(position* p);
int strategie_hasard(position* pos);

#endif // IA_H_
