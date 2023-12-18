#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jeu.h"

int main(void){
  plateau* p1 = initialise(6);
  affiche(p1);
  return 0;
}
