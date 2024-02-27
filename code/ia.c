#include "ia.h"

int nb_pierres_restantes_joueur(position* pos, int joueur){
    int indice = 0;
    int tailleRangee = pos->plateau->tailleRangee;
    assert(joueur == 1 || joueur == 2);
    if(joueur == 2){
        indice = tailleRangee;
    }
    int nb_pierres = 0;
    for(int i = indice; i < indice + tailleRangee; i++){
        nb_pierres += pos->plateau->pierres[i];
    }
    return nb_pierres;
}

bool puit_jouable(position* p, int puit){
  /* Verifie que le joueur peut jouer le puit p sans mettre en etat de famine son adversaire */
  assert(puit < p->plateau->tailleRangee * 2);
  if(p->plateau->pierres[puit] == 0){
    return false;
  }
  position* copie = p;
  int puit_arrivee = semer(copie->plateau, puit);
  recolte(copie->plateau, puit_arrivee);
  if(nb_pierres_restantes_joueur(copie, 3 - p->joueur) > 0){
      return true;
  }
  return false;
  // Penser a free la copie
}



bool partie_jouable(position* pos){
   // Regarde si on peut nourrir le joueur <joueur>
   int indiceDepart = 0;
   int tailleRangee = pos->plateau->tailleRangee;
    if(pos->joueur == 2){
        indiceDepart = tailleRangee;
    }
    for(int i = indiceDepart; i < indiceDepart + tailleRangee; i++){
        if(puit_jouable(pos, i)){
            return true;
        }
    }
    return false;
}



bool terminale(position* pos){
  int nb_pierres_gagnantes = nb_total_pierres(pos->plateau) / 2;
  if(!partie_jouable(pos)){
    return true;
  }else if(pos->nb_pierres_j1 > nb_pierres_gagnantes || pos->nb_pierres_j2 > nb_pierres_gagnantes){
    return true;
  }
  return false;
}

position* initialiser_position(plateau* p, int j){
  // Creer une position avec le plateau p ou c'est a j de jouer
  // On initialise les compteurs de chacuns des joueurs a 0
  position* res = malloc(sizeof(position));
  res->plateau = p;
  res->joueur = j;
  res->nb_pierres_j1 = 0;
  res->nb_pierres_j2 = 0;
  return res;
}

int jouer_coup(position* p, int puit){
  /* Modifie la position en semant les graines du <puit> */
  /* On change de joueur a la fin de la fonction */
  int taille_rangee = p->plateau->tailleRangee;
  if(puit < 0 || puit >= 2 * taille_rangee){
    assert(false);
  }
  else if(puit >= taille_rangee){
    assert(p->joueur == 2);
  }else{
    assert(p->joueur == 1);
  }
  int puit_arrive = semer(p->plateau, puit);
  int graines_gagnees = recolte(p->plateau, puit_arrive);
  printf("[info] > Le joueur %d a joue le puit %d et a remporte %d pierres\n", p->joueur, puit, graines_gagnees);
  if(graines_gagnees > 0){
    if(p->joueur == 1){p->nb_pierres_j1 += graines_gagnees;}
    if(p->joueur == 2){p->nb_pierres_j2 += graines_gagnees;}
  }
  p->joueur = 3 - p->joueur;
  return graines_gagnees;
}

void afficher_position(position* p){
  printf("\n");
  printf("########## AFFICHAGE DE LA POSITION ##########\n");
  affiche_jeu(p->plateau);
  printf("\n");
  printf("Nombre de pierres joueur 1 : %d\nNombre de pierres joueur 2 : %d\n",p->nb_pierres_j1, p->nb_pierres_j2);
  printf("C'est au joueur %d de jouer !\n", p->joueur);
  printf("##############################################\n");
  printf("\n");
}


int* indice_puits_non_vides(position* pos){
  /* Renvoie un tableau avec les indices des puits non vides */
  /* Le tableau commence a 1, la case 0 contient la taille */
  int taille_rangee = pos->plateau->tailleRangee;
  int nb_puits_non_vide = 0;
  int indice = 0;
  if(pos->joueur == 2){
    indice = taille_rangee;
  }
  for(int i = 0; i < taille_rangee; i++){
    if(pos->plateau->pierres[indice + i] > 0){
      nb_puits_non_vide++;
    }
  }
  int* indice_puits = malloc(sizeof(int)* (nb_puits_non_vide + 1));
  indice_puits[0] = nb_puits_non_vide;
  int compteur = 1;
  for(int i = 0; i < taille_rangee; i++){
    if(pos->plateau->pierres[indice + i] > 0){
      indice_puits[compteur] = indice + i;
      compteur++;
    }
  }
  return indice_puits;
}

int strategie_hasard(position* pos){
  int* indice_puits = indice_puits_non_vides(pos);
  int taille = indice_puits[0];
  int rand_indice = (rand()%taille) + 1;
  int rand_puit = indice_puits[rand_indice];
  return rand_puit;
}

int gagnant(position* pos){
  // Entree : une position terminale
  // Sortie : 1 Si le joueur 1 gagne, 2 si c'est le joueur 2, 0 si nul
  assert(terminale(pos));
  if(pos->nb_pierres_j1 > pos->nb_pierres_j2){
    return 1;
  }else if(pos->nb_pierres_j2 > pos->nb_pierres_j1){
    return 2;
  }else{
    return 0;
  }
}
