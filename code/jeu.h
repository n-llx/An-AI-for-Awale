#ifndef JEU_H_
#define JEU_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct plateau {
  int tailleRangee;
  int* pierres;
};
typedef struct plateau plateau;

plateau* initialise_jeu(int tailleRangee);
void affiche_jeu(plateau* p);
void distribueInitial(plateau* p, int* distribution_pierre, int n);
int semer(plateau* p, int puit);
int recolte(plateau* p, int puit);
plateau* lectureEntree_jeu(const char* nomFichier);

#endif // JEU_H_
