#ifndef PARTIE_H_
#define PARTIE_H_
#include "ia.h"

int jouer_partie(int (*strat1)(position*), int (*strat2)(position*), bool afficher);

#endif // PARTIE_H_
