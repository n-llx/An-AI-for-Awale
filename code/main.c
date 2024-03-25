#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"

int main(void){
  srand(time(NULL));
  plateau* p = lectureEntree_jeu("grilles/grille1.txt");
  position* pos = initialiser_position(p, 1);
  jouer_partie(pos, strategie_min_max_h1, strategie_hasard, true);
  return 0;
}
