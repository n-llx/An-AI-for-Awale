#ifndef IA_H_
#define IA_H_
#include "jeu.h"

struct position {
    plateau* plateau;
    int joueur;
    int nb_pierres_j1;
    int nb_pierres_j2;
}; // Represente un sommet du graphe du jeu 
typedef struct position position;

bool terminale(position* pos); // Renvoie vraie si la position est terminale
position* initialiser_position(plateau* p, int j);
int jouer_coup(position* p, int puit);
void afficher_position(position* p);
int strategie_hasard(position* pos);
int strategie_joueur(position* pos);
int gagnant(position* pos);
int jouer_partie(position* pos, int (*strat1)(position*), int (*strat2)(position*), bool afficher);
int jouer_partie_debut(int (*strat1)(position*), int (*strat2)(position*), bool afficher);
void jouer_tant_que_1_gagne(int (*strat1)(position*), int (*strat2)(position*));
int strategie_min_max_h1(position* pos);
double* ratioVictoire(int (*strat1)(position*), int (*strat2)(position*), int nb_parties);
void afficherRatio(double* ratio);
#endif // IA_H_
