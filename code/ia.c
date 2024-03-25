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
  //printf("[<jouer_coup>] > Le joueur %d a joue le puit %d et a remporte %d pierres\n", p->joueur, puit, graines_gagnees);
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
  int* t = p->plateau->pierres;
  printf("\n");
  printf("\033[38;5;8m       11   10    9    8    7    6\n\033[0m");
  printf("┌────┬────┬────┬────┬────┬────┬────┬────┐\n");
  printf("│    │ %2d │ %2d │ %2d │ %2d │ %2d │ %2d │    │\n",t[11],t[10],t[9],t[8],t[7],t[6]);
  printf("│ %2d ├────┼────┼────┼────┼────┼────┤ %2d │\n", p->nb_pierres_j2, p->nb_pierres_j1);
  printf("│    │ %2d │ %2d │ %2d │ %2d │ %2d │ %2d │    │\n",t[0],t[1],t[2],t[3],t[4],t[5]);
  printf("└────┴────┴────┴────┴────┴────┴────┴────┘\n");
  printf("\033[38;5;8m        0    1    2    3    4    5\n\033[0m");
 printf("C'est au joueur %d de jouer !\n", p->joueur);
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
  if(!terminale(pos)){ // Debug
    afficher_position(pos); // Debug
  } // Debug
  assert(terminale(pos));
  if(pos->nb_pierres_j1 > pos->nb_pierres_j2){
    return 1;
  }else if(pos->nb_pierres_j2 > pos->nb_pierres_j1){
    return 2;
  }else{
    return 0;
  }
}

int heuristique_naive(position* pos){
  // Entree : une position
  // Sortie : la valeur de l'heuristique pour le joueur 1
  return pos->nb_pierres_j1 - pos->nb_pierres_j2;
}

int indice_min_tab(int* tab, int taille){
  int i_min = -1;
  int min = 100000000;
  for(int i = 0; i < taille; i++){
    if(tab[i] < min){
      min = tab[i];
      i_min = i;
    }
  }
  return i_min;
}

int indice_max_tab(int* tab, int taille){
  int i_max = -1;
  int max = -100000000;
  for(int i = 0; i < taille; i++){
    if(tab[i] > max){
      max = tab[i];
      i_max = i;
    }
  }
  return i_max;
}

int min_max_heuristique(position* pos, int (*heuristique)(position*), int d){
  // Entree : une position pos, une heuristique et une profondeur d
  // Sortie : Le meilleur coup a jouer en evaluant l'arbre a une profondeur
  // d en utilisant l'heuristique
  if(terminale(pos)){
    if(gagnant(pos) == 1){
      return 100;
    }else if(gagnant(pos) == 2){
      return -100;
    }else{
      return 0;
    }
  }else if(d == 0){
    return heuristique(pos);
  }else{
    // On creer un tableau de taille 6 ou les cases 
    // correspondent au meilleur score puis on maximise ou minimise
    int* score_coup = malloc(sizeof(int) * 6);
    for(int i = 0; i < 6; i++){
      score_coup[i] = 0;
    }

    int si_puit_j2 = 0;
    if(pos->joueur == 2){si_puit_j2 = 6;}
    for(int i = 0; i < 6; i++){
      if(puit_jouable(pos, si_puit_j2 + i)){
        position* copie = copie_pos(pos);
        jouer_coup(copie, si_puit_j2 + i);
        score_coup[i] = min_max_heuristique(copie, heuristique, d-1);
        liberer_position(copie);
      }else{
        if(pos->joueur == 1){score_coup[i] = -1;}else{score_coup[i] = 1;}
      }
    }
    if(pos->joueur == 1){
      int foo = indice_max_tab(score_coup, 6) + si_puit_j2;
      printf("Nous sommes a une profondeur %d\n", d); //debug
      for(int i = 0; i < 6; i++){ //debug
        printf("Score du coup %d=%d\n ", i+si_puit_j2, score_coup[i]); //debug
      } //debug
      printf("\n"); //debug
      return foo;
    }else if(pos->joueur == 2){
      int foo = indice_min_tab(score_coup, 6) + si_puit_j2;
      printf("Nous sommes a une profondeur %d\n", d); //debug
      for(int i = 0; i < 6; i++){ //debug
        printf("Score du coup %d=%d\n ", i+si_puit_j2, score_coup[i]); //debug
      } //debug
      printf("\n"); //debug
      return foo;
    }else{
      printf("Erreur dans <min_max_heuristique>\n");
      return -1;
    }
  }
}

int strategie_min_max_h1(position* pos){
  int coup = min_max_heuristique(pos, heuristique_naive, 3);
  return coup;
}

int jouer_partie(position* pos, int (*strat1)(position*), int (*strat2)(position*), bool afficher){
   // Entree : deux strategies pour le joueur 1 et le joueur 2
   // Sortie : On affiche le déroulé de la partie et on renvoie le joueur gagnant
   int compteur = 0;
    while(!terminale(pos) && compteur <= 1000){
       if(pos->joueur == 1){
            int coup = strat1(pos);
            if(afficher){printf("Le joueur 1 vient de jouer le puit %d\n",coup);}
            jouer_coup(pos, coup);
        }else if(pos->joueur == 2){
          int coup = strat2(pos);
          if(afficher){printf("Le joueur 2 vient de jouer le puit %d\n",coup);}
          jouer_coup(pos, coup);
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
    if(compteur > 1000){
      return 0;
    }else{
      return gagnant(pos);
    }
}

int jouer_partie_debut(int (*strat1)(position*), int (*strat2)(position*), bool afficher){
  plateau* plat = lectureEntree_jeu("grilles/grille0.txt");
  position* pos = initialiser_position(plat, 1);
  int gagnant = jouer_partie(pos, strat1, strat2, afficher);
  liberer_position(pos);
  return gagnant;
}

double* ratioVictoire(int (*strat1)(position*), int (*strat2)(position*), int nb_parties){
  // Entree : Deux strategies et un nombre de partie a disputer
  // Sorite : Un tableau de taille 3 ou la premiere case est le nb de parties
  // gagnee avec la strategie 1 puis le nb de parties perdues et le nb de
  // nulles
  double* res = malloc(sizeof(double) * 3);
  for(int i = 0; i < 3; i++){
    res[i] = 0.;
  }
  for(int i = 0; i < nb_parties; i++){
    plateau* plat = lectureEntree_jeu("grilles/grille0.txt");
    position* pos = initialiser_position(plat, 1);
    int joueur_gagnant = jouer_partie(pos, strat1, strat2, false);
    if(joueur_gagnant == 1){
      res[0] = res[0] + 1.;
    }else if(joueur_gagnant == 2){
      res[1] = res[1] + 1.;
    }else if(joueur_gagnant == 0){
      res[2] = res[2] + 1.;
    }else{
      fprintf(stderr, "Erreur dans la fonction <ratioVictoire>\n");
      return NULL;
    }
  }
  for(int i = 0; i < 3; i++){
    res[i] = res[i] / (double)nb_parties;
  }
  return res;
}

void afficherRatio(double* ratio){
  printf("Nombre de victoires de la strategie 1 : %.1f%%\n", ratio[0]*100);
  printf("Nombre de defaites de la strategie 1 : %.1f%%\n", ratio[1]*100);
  printf("Nombre de parties egalite : %.1f%%\n", ratio[2]*100);
}

