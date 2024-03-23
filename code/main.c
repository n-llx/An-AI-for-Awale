#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"

int main(void){
  srand(time(NULL));
  jouer_partie_debut(strategie_min_max_h1, strategie_hasard, true);
  return 0;
}
