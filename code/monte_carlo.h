#ifndef MONTE_CARLO_H_
#define MONTE_CARLO_H_ 

#include "ia.h"
#define CONTRAINTE 10000
struct node {
  position* pos;
  int nb_visites;
  double ratio_victoire;
  struct node* parent;
  struct node* enfants[6];
};
typedef struct node node;

void test(position* pos);

#endif // MONTE_CARLO_H_
