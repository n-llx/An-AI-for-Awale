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

position* creer_position_fichier(char* nom_fichier, int joueur, int nb_pierre_1, int nb_pierre_2);
void liberer_position(position* p);
position* copie_pos(position* pos);
bool puit_jouable(position* p, int puit);
bool terminale(position* pos); // Renvoie vraie si la position est terminale
position* initialiser_position(plateau* p, int j);
void afficher_position(position* p);
int coup_aleatoire_possible(position* p);
int strategie_hasard(position* pos);
int strategie_joueur(position* pos);
int gagnant(position* pos);
int jouer_coup(position* p, int puit);
int jouer_partie(position* pos, int (*strat1)(position*), int (*strat2)(position*), bool afficher);
int jouer_partie_debut(int (*strat1)(position*), int (*strat2)(position*), bool afficher);
int strategie_min_max_h1(position* pos);
void afficherRatio(int (*strat1)(position*), int (*strat2)(position*), int nb_parties);
#endif // IA_H_
