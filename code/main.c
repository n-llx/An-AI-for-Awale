#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"
#include "monte_carlo.h"

int main(void){
  //  srand(time(NULL));
  srand(6);
  plateau* p = lectureEntree_jeu("grilles/grille2.txt");
  position* pos = initialiser_position(p, 1);
  int coup = strategie_mcts(pos);
  return 0;
}

