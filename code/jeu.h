#ifndef JEU_H_
#define JEU_H_
struct plateau {
  int tailleRangee;
  int* pieres;
};
typedef struct plateau plateau;

plateau* initialise(int tailleRangee);
void affiche(plateau* p);
#endif // JEU_H_
