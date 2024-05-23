#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"
#include "flat_mc.h"
#include "mcts.h"

int main(void){
  srand(time(NULL));
  //  srand(0);
  afficherRatio(strategie_mcts, strategie_hasard, 100);

  return 0;
}

