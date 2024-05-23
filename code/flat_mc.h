#ifndef FLAT_MC_H_
#define FLAT_MC_H_ 

#include "ia.h"
#include <time.h>
#define CONTRAINTE 500
struct node {
  int id;
  position* pos;
  int nb_visites;
  int nb_victoires;
  struct node* parent;
  struct node* enfants[6];
};
typedef struct node node;

int strategie_flat_mc(position* p);
node* flat_mc(position* p);

#endif // FLAT_MC_H_
