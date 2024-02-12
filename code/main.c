#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jeu.h"

int main(void){
  plateau* p1 = lectureEntree_jeu("grilles/grille4.txt");
  affiche_jeu(p1);
  semer(p1, 0);
  affiche_jeu(p1);
  return 0;
}
