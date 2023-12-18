#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jeu.h"

plateau* initialise(int tailleRangee){
  plateau* res = malloc(sizeof(plateau));
  res->tailleRangee = tailleRangee;
  int* pieres = malloc(sizeof(int) * tailleRangee * 2);
  for(int i = 0; i < 2*res->tailleRangee; i++){
    pieres[i] = 0;
  }
  res->pieres = pieres;
  return res;
}

void affiche(plateau* p){
  for(int i = 0; i < 2*p->tailleRangee; i++){
    if(i == p->tailleRangee){
      printf("\n");
    }
    printf("%d ", p->pieres[i]);
  }
  printf("\n");
}
