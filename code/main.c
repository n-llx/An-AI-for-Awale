#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"
#include "flat_mc.h"
#include "mcts.h"
#include "retrograde.h"

int main(void){
  srand(time(NULL));
  //  srand(0);
  afficherRatio(strategie_min_max_h1,strategie_mcts, 50);
  return 0;
}

