#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "jeu.h"
#include "ia.h"
#include "partie.h"

int main(void){
  jouer_partie(strategie_hasard, strategie_hasard, true);
  return 0;
}
