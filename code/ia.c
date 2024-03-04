#include "ia.h"

int nb_pierres_restantes_joueur(position* pos, int joueur){
  // Entree : Une position et un joueur j
  // Sortie : Renvoie le nombre de pierre dans les trous du joueur j
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

position* copie_pos(position* pos){
  // Entree : Une position p
  // Sortie : Une copie de p avec une memoire nouvellement alloue
  position* res = malloc(sizeof(position));
  plateau* c_plat = malloc(sizeof(plateau));
  c_plat->tailleRangee = pos->plateau->tailleRangee;
  int* c_pierres = malloc(sizeof(int) * 2 * c_plat->tailleRangee);
  for(int i = 0; i < 2*c_plat->tailleRangee; i++){
    c_pierres[i] = pos->plateau->pierres[i];
  }
  c_plat->pierres = c_pierres;
  res->joueur = pos->joueur;
  res->nb_pierres_j1 = pos->nb_pierres_j1;
  res->nb_pierres_j2 = pos->nb_pierres_j2;
  res->plateau = c_plat;
  return res;
}

void liberer_position(position* p){
  // Entree : Une position
  // Sortie : Libere la memoire alloue a la position
  liberer_plateau(p->plateau);
  free(p);
}

bool puit_jouable(position* p, int puit){
  // Entree : Une position p et un entier qui le numero du puit
  // Sortie : Vrai si le joueur peut jouer le puit p sans mettre 
  // l'adversaire dans un etat de famine
  assert(puit < p->plateau->tailleRangee * 2);
  if(p->plateau->pierres[puit] == 0){
    return false;
  }
  position* copie = copie_pos(p);
  int puit_arrivee = semer(copie->plateau, puit);
  recolte(copie->plateau, puit_arrivee);
  if(nb_pierres_restantes_joueur(copie, 3 - p->joueur) > 0){
    liberer_position(copie);
    return true;
  }
  liberer_position(copie);
  return false;
}



bool partie_jouable(position* pos){
  // Entree : une position
  // Sortie : Vrai si le joueur actuel peut nourrir son adversaire
  // en semant un de ses puits
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
  // Entree : une position
  // Sortie : Vrai si le joueur p->joueur ne peut plus jouer ou
  // si un des deux joueurs a gagne, faux sinon
  int nb_pierres_gagnantes = nb_total_pierres(pos->plateau) / 2;
  if(!partie_jouable(pos)){
   return true;
  }else if(pos->nb_pierres_j1 > nb_pierres_gagnantes || pos->nb_pierres_j2 > nb_pierres_gagnantes){
    return true;
  }
  return false;
}

position* initialiser_position(plateau* p, int j){
  // Entree : un plateau et un joueur
  // Sorite : Une position correspond au plateau, ou c'est au joueur
  // j de jouer. Aucun joueur n'a remporte de pierres
  position* res = malloc(sizeof(position));
  res->plateau = p;
  res->joueur = j;
  res->nb_pierres_j1 = 0;
  res->nb_pierres_j2 = 0;
  return res;
}

int jouer_coup(position* p, int puit){
  // Entree : Une position et un entier qui correspond au puit joue
  // Sortie : La position p a ete change, on passe a une position 
  // qui appartient au joueur adverse et on renvoie le nombre de
  // pierres gagnees.
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
 int graines_gagnees = 0;
 // Il ne faut recolter que si on est dans le camp adverse :
 if(p->joueur == 1){
  if(puit_arrive >= taille_rangee){
    graines_gagnees = recolte(p->plateau, puit_arrive);
  }
 }else if(p->joueur == 2){
   if(puit_arrive < taille_rangee){
     graines_gagnees = recolte(p->plateau, puit_arrive);
   }
 }
  printf("[<jouer_coup>] > Le joueur %d a joue le puit %d et a remporte %d pierres\n", p->joueur, puit, graines_gagnees);
  if(graines_gagnees > 0){
    if(p->joueur == 1){p->nb_pierres_j1 += graines_gagnees;}
    if(p->joueur == 2){p->nb_pierres_j2 += graines_gagnees;}
  }
  p->joueur = 3 - p->joueur;
  return graines_gagnees;
}

void afficher_position(position* p){
  // Entree : une position 
  // Sortie : Affichage sur la sortie du plateau, du score et du joueur
  // auquel c'est de jouer
  printf("\n");
  printf("┌────┬────┬────┬────┬────┬────┬────┬────┐\n");
  printf("│    │ %2d │ %2d │ %2d │ %2d │ %2d │ %2d │    │\n",0,11,4,5,6,7);
  printf("│ %2d ├────┼────┼────┼────┼────┼────┤ %2d │\n", 25, 3);
  affiche_jeu(p->plateau);
  printf("\n");
  printf("Nombre de pierres joueur 1 : %d\nNombre de pierres joueur 2 : %d\n",p->nb_pierres_j1, p->nb_pierres_j2);
  printf("C'est au joueur %d de jouer !\n", p->joueur);
  printf("##############################################\n");
  printf("\n");
}


int* indice_puits_non_vides(position* pos){
  // Entree : une position
  // Sortie : Un tableau ou la case 0 est la taille du tableau et
  // les cases suivantes continennent les indices des puits jouables
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
  // Entree : Une position
  // Sortie : Un puit au hasard parmi ceux qui sont jouables
  int* indice_puits = indice_puits_non_vides(pos);
  int taille = indice_puits[0];
  int rand_indice = (rand()%taille) + 1;
  int rand_puit = indice_puits[rand_indice];
  return rand_puit;
}

int strategie_joueur(position* pos){
  // Entree : Une position pos 
  // Sortie : Le puit que le joueur souhaite semer 
  int indice_puit = -1;
  printf("Quelle puit voulez vous semer ?\n");
  scanf("%d", &indice_puit);
  assert(indice_puit >= 0 && indice_puit < pos->plateau->tailleRangee * 2);
  return indice_puit;
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

int jouer_partie(position* pos, int (*strat1)(position*), int (*strat2)(position*), bool afficher){
   // Entree : deux strategies pour le joueur 1 et le joueur 2
   // Sortie : On affiche le déroulé de la partie et on renvoie le joueur gagnant
   int compteur = 0;
    while(!terminale(pos) && compteur <= 1000){
       if(pos->joueur == 1){
            jouer_coup(pos, strat1(pos));
        }else if(pos->joueur == 2){
            jouer_coup(pos, strat2(pos));
        }
        else{assert(false);}
        if(afficher){
          if(terminale(pos)){
            printf("[<jouer_partie>] : Le joueur %d a gagne !\n", gagnant(pos));
          }else{
            afficher_position(pos);
          }
        }
        compteur++;
    }
    return gagnant(pos);
}
