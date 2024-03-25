#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"

int main(void){
  srand(time(NULL));
  afficherRatio(ratioVictoire(strategie_hasard, strategie_min_max_h1, 100));
  return 0;
}
