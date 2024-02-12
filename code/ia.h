#ifndef IA_H_
#define IA_H_
#include "jeu.h"

struct position {
    plateau* plateau;
    int joueur;
};
typedef struct position position;

#endif // IA_H_
