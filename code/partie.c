#include "partie.h"

int jouer_partie(int (*strat1)(position*), int (*strat2)(position*), bool afficher){
   // Entree : deux strategies pour le joueur 1 et le joueur 2
   // Sortie : On affiche le déroulé de la partie et on renvoie le joueur gagnant
    plateau* plateau = lectureEntree_jeu("grilles/grille0.txt");
    position* pos = initialiser_position(plateau, 1);
    while(!terminale(pos)){
        if(pos->joueur == 1){
            jouer_coup(pos, strat1(pos));
        }else if(pos->joueur == 2){
            jouer_coup(pos, strat2(pos));
        }
        else{assert(false);}
        if(afficher){
            afficher_position(pos);
        }
    }
    return gagnant(pos);
}
