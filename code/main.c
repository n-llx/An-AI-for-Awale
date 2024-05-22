#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"
#include "monte_carlo.h"

int main(void){
  srand(time(NULL));
  //srand(6);

  plateau* p = lectureEntree_jeu("grilles/grille1.txt");
  position* pos = initialiser_position(p, 1);
  pos->nb_pierres_j1 = 6;
  pos->nb_pierres_j2 = 10;
  node* arbre = mcts(pos);
  affichage_arbre(arbre);
  
  // afficherRatio(ratioVictoire(strategie_mcts, strategie_min_max_h1, 10));
  return 0;
}

