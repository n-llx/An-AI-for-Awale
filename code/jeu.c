#include "jeu.h"

plateau* initialise_jeu(int tailleRangee){
  /*
    * initialise(6) renvoie un tableau a une dimension de cette forme
    * 11 10 9 8 7 6
    * 0  1  2 3 4 5
    * */
  plateau* res = malloc(sizeof(plateau));
  res->tailleRangee = tailleRangee;
  int* pierres = malloc(sizeof(int) * tailleRangee * 2);
  for(int i = 0; i < 2*res->tailleRangee; i++){
    pierres[i] = 0;
  }
  res->pierres = pierres;
  return res;
}

int nb_total_pierres(plateau* p){
  return p->tailleRangee * 2 * 4;
}

void affiche_jeu(plateau* p){
  // Permet d'afficher le plateau en 2 rangees distinctes
  for(int i = 0; i < p->tailleRangee; i++){
    printf(" %d |", p->pierres[2 * p->tailleRangee - i - 1]);
  }
  printf("\n");
  for(int i = 0; i < p->tailleRangee; i++){
    printf(" %d |", p->pierres[i]);
  }
  printf("\n");
  printf("--------------------------------------------------");
}

void distribueInitial(plateau* p, int* distribution_pierres, int n){
  // Remplie le tableau avec une certaine distribution initiale
  assert(n == p->tailleRangee * 2);
  for(int i = 0; i < p->tailleRangee * 2; i++){
    p->pierres[i] = distribution_pierres[i];
  }
}

int semer(plateau* p, int puit){
  // Seme les graines du puit p et renvoie le puit sur lequel on s'est arrete
  int tailleTab = p->tailleRangee * 2;
  assert(puit < tailleTab);
  int nb_pierre_a_semer = p->pierres[puit];
  p->pierres[puit] = 0;
  int i = (puit + 1)%tailleTab; // On commence a partir du puit i
  int nbTours = nb_pierre_a_semer / (2*p->tailleRangee - 1); // On determine le nombre de pierres que toutes les cases vont avoir
  nb_pierre_a_semer = nb_pierre_a_semer % (2*p->tailleRangee - 1);
  if(nbTours > 0){ // On fait au moins un tour de plateau
    for(int j = i; j != (i-1)%tailleTab; j = (j+1)%tailleTab){
      p->pierres[j] = p->pierres[j] + nbTours;
    } // Quand on finit on arrive sur la case i ie la case du puit initial
  }
  for(; nb_pierre_a_semer > 0; i = (i+1)%tailleTab){
    p->pierres[i] = p->pierres[i] + 1;
    nb_pierre_a_semer--;
  }
  return (i+tailleTab-1)%tailleTab;
}

int recolte(plateau* p, int puit){
  //Recolte les graines a partir du puit p et renvoie la somme des graines recoltees
  int somme = 0;
  int puit_regarde = puit;
  // La condition dans la boucle teste si on est au dernier puit d'une rangee
  do {
    if(p->pierres[puit_regarde] == 2 || p->pierres[puit_regarde] == 3){
    somme += p->pierres[puit_regarde];
    p->pierres[puit_regarde] = 0;
    puit_regarde--;
    }else{
      break;
    }
  }while(((puit_regarde != (p->tailleRangee - 1)) && (puit_regarde != -1)));
  return somme;
}

plateau* lectureEntree_jeu(const char* nomFichier){
  // Lit une grille dans un fichier et renvoie le plateau correspondant correctement initialise
  FILE* fichier = fopen(nomFichier, "r");
  if(fichier == NULL){
    fprintf(stderr, "Erreur dans la fonction 'lecture_entree' : le fichier n'a pas pu etre ouvert.\n");
  }
  int nbPuitsRangee = -1;
  fscanf(fichier, "%d ", &nbPuitsRangee);
  assert(nbPuitsRangee != -1);
  int* pierres = malloc(sizeof(int) * 2 * nbPuitsRangee);
  for(int i = 0; i < nbPuitsRangee; i++){
    fscanf(fichier, "%d ", &pierres[2*nbPuitsRangee - i - 1]);
  }
  for(int i = 0; i < nbPuitsRangee; i++){
    fscanf(fichier, "%d", &pierres[i]);
  }
  plateau* res = initialise_jeu(nbPuitsRangee);
  res->pierres = pierres;
  return res;
}

void liberer_plateau(plateau* p){
  free(p->pierres);
  free(p);
}
