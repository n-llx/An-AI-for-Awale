#ifndef MCTS_H_
#define MCTS_H_

#include "ia.h"
#include <math.h>
#define CONSTANTE 0.70711
// = 1/sqrt(2)
// #define CONTRAINTE_MCTS 1000

struct noeud {
  int id;
  position* pos;
  int nb_visites;
  double Q;
  struct noeud* parent;
  struct noeud* enfants[6];
};
typedef struct noeud noeud;

int strategie_mcts(position* pos);
void test_mcts();

#endif // MCTS_H_
