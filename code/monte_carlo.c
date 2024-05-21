#include "monte_carlo.h"

node* creer_noeud(position* pos){
  // Entree : Unen position ie un etat du jeu
  // Sortie : Un noeud de l'arbre de recherche ayant un nb de visites et
  // un winrate de 0 avec des fils nuls
  node* res = malloc(sizeof(node));
  res->pos = pos;
  res->nb_visites = 0;
  res->ratio_victoire = 0.;
  res->parent = NULL;
  for(int i = 0; i < 6; i++){
    res->enfants[i] = NULL;
  }
  return res;
}

node* regle_arbre_aleatoire(node* tree){
  // Entree : La racine d'un arbre
  // Sortie : un noeud choisi au hasard dont il reste un coup a jouer
  for(int i = 0; i < 6; i++){
    int index_random = rand()%6;
    if(tree->enfants[index_random] == NULL){
      return tree;
    }else{
      return regle_arbre_aleatoire(tree->enfants[index_random]);
    }
  }
  assert(false);
  return NULL;
}



node* developper(node* n){
  // Entree : un noeud n
  // Sortie : renvoie un noeud accessible depuis n par un coup ou NULL si
  // impossible
  if(terminale(n->pos)){return NULL;}
  int si_joueur_2 = 0;
  if(n->pos->joueur == 2){si_joueur_2 = 6;}
  int coup = coup_aleatoire_possible(n->pos);
  if(coup == -1){
    printf("Dans monte_carlo.c, developper : Le coup est impossible\n");
    return NULL;
  }
  position* new_pos = copie_pos(n->pos);
  jouer_coup(new_pos, coup + si_joueur_2);
  node* new = creer_noeud(new_pos);
  new->parent = n;
  n->enfants[coup] = new;
  return new;
}

int regle_default(node* noeud){
  // Entree : Un noeud
  // Sortie : Joue une partie au hasard a partir de la position du noeud
  // et renvoie le score
  position* copie = copie_pos(noeud->pos);
  int gagnant = jouer_partie(copie, strategie_hasard, strategie_hasard, false);
  liberer_position(copie);
  if(gagnant == noeud->pos->joueur){
    return 1;
  }else if(gagnant == 3 - noeud->pos->joueur){
    return -1;
  }else{
    return 0;
  }
}

void retropropagation(node* noeud){
  // Entree : Un noeud
  // Sortie : Propage le score de la partie sur tout les parents du noeuds
  int score = regle_default(noeud);
  while(noeud != NULL){
    noeud->nb_visites++;
    if(noeud->nb_visites == 1){
      noeud->ratio_victoire = (double)score;
    }else{
      noeud->ratio_victoire = (noeud->ratio_victoire + score) / 2.;
    }
    noeud = noeud->parent;
  }
}

node* mcts(position* pos){
  node* racine = creer_noeud(pos);
  for(int i = 0; i < CONTRAINTE; i++){
    node* a_explorer = regle_arbre_aleatoire(racine);
    node* fils_a_explorer = developper(a_explorer);
    if(fils_a_explorer != NULL){
      retropropagation(fils_a_explorer);
    }else{
      printf("<function mcts> : Le coup est impossible\n");
    }
  }
  return racine;
}

void test(position* pos){
  node* racine = mcts(pos);
  printf("Affichage des fils de la racine\n");
  for(int i = 0; i < 6; i++){
    if(racine->enfants[i] != NULL){
      printf("Nombre de visites pour le noeud %d: %f\n",i, racine->enfants[i]->ratio_victoire);
    }
  }
}
