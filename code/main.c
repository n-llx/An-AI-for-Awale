#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"

int main(void){
  srand(time(NULL));
  plateau* plat = lectureEntree_jeu("grilles/grille0.txt");
  position* pos = initialiser_position(plat, 1);
  afficher_position(pos);
  jouer_partie(pos, strategie_joueur, strategie_hasard, true);
  return 0;
}
