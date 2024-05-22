#ifndef MONTE_CARLO_H_
#define MONTE_CARLO_H_ 

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

int strategie_mcts(position* p);

#endif // MONTE_CARLO_H_
