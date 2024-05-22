#include "monte_carlo.h"

node* creer_noeud(position* pos){
  // Entree : Unen position ie un etat du jeu
  // Sortie : Un noeud de l'arbre de recherche ayant un nb de visites et
  // un winrate de 0 avec des fils nuls
  node* res = malloc(sizeof(node));
  res->id = -1;
  res->pos = pos;
  res->nb_visites = 0;
  res->nb_victoires = 0;
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
    int coup_aleatoire = coup_aleatoire_possible(tree->pos);
    if(tree->enfants[coup_aleatoire] == NULL || coup_aleatoire == -1){
      // Si on peut jouer un des coups ou si tree est terminal
      return tree;
    }else{
      node* noeud_suivant = tree->enfants[coup_aleatoire];
      return regle_arbre_aleatoire(noeud_suivant);
    }
  }
  assert(false);
  return NULL;
}

int renvoyer_hasard_coup_restant(node* n){
  // Entree : un noeud
  // Sortie : Un entier correspondant a un coup au hasard parmi ceux qui n'ont
  // pas encore ete joue et qui sont jouable
  bool coups_restants_possible[6] = {false};
  int si_joueur_2 = 0;
  if(n->pos->joueur == 2){si_joueur_2 = 6;}
  int compte = 0;
  for(int i = 0; i < 6; i++){
    if(n->enfants[i] == NULL && puit_jouable(n->pos, i + si_joueur_2)){
      coups_restants_possible[i] = true;
      compte++;
    }
  }
  assert(compte != 0);
  int random_index = rand()%compte;
  for(int i = 0; i < 6; i++){
    if(coups_restants_possible[i] && random_index == 0){
      return i;
    }else if(coups_restants_possible[i]){
      random_index--;
    }
  }
  assert(false);
  return -1;
}

node* developper(node* n){
  // Entree : un noeud n
  // Sortie : renvoie un noeud accessible depuis n par un coup ou NULL si
  // impossible
  if(terminale(n->pos)){
    assert(false);
    return NULL;
  }
  int si_joueur_2 = 0;
  if(n->pos->joueur == 2){si_joueur_2 = 6;}
  int coup = renvoyer_hasard_coup_restant(n);
  if(coup == -1){
    printf("ERREUR : Dans monte_carlo.c, developper : Le coup est impossible\n");
    return NULL;
  }
  position* new_pos = copie_pos(n->pos);
  jouer_coup(new_pos, coup + si_joueur_2);
  node* new = creer_noeud(new_pos);
  new->parent = n;
  n->enfants[coup] = new;
  return new;
}

int regle_default(node* noeud, int joueur){
  // Entree : Un noeud
  // Sortie : Joue une partie au hasard a partir de la position du noeud
  // et renvoie le score
  position* copie = copie_pos(noeud->pos);
  int gagnant = jouer_partie(copie, strategie_hasard, strategie_hasard, false);
  liberer_position(copie);
  if(gagnant == joueur){
    return 1;
  }else{
    return 0;
  }
}

void retropropagation(node* noeud, int joueur){
  // Entree : Un noeud d'ou on a lance la partie
  // Sortie : Propage le score de la partie sur tout les parents du noeuds
  int score = regle_default(noeud, joueur);
  while(noeud != NULL){
    noeud->nb_visites++;
    noeud->nb_victoires += score;
    noeud = noeud->parent;
  }
}

node* mcts(position* pos){
  // Entree : Une position du jeu
  // Sortie : Un arbre de Monte Carlo itere avec la CONTRAINTE
  node* racine = creer_noeud(pos);
  for(int i = 0; i < CONTRAINTE; i++){
    assert(racine != NULL);
    node* a_explorer = regle_arbre_aleatoire(racine);
    if(terminale(a_explorer->pos)){
      retropropagation(a_explorer, racine->pos->joueur); // On n'ajoute pas de noeud si la partie est terminale
    }else{
      node* fils_a_explorer = developper(a_explorer);
      assert(fils_a_explorer != NULL);
      retropropagation(fils_a_explorer, racine->pos->joueur);
    }
  }
  return racine;
}

int couleur_gris(node* n){
  // Entree : Un noeud
  // Sortie : Un nombre dans [|0;100|] qui correspond a une nuance de gris
  // choisie par rapport au winrate
  return 100 -(int)(((double)n->nb_victoires/(double)n->nb_visites)*100);
}
  
void parcours_en_profondeur(node* racine, FILE* f, int* numero_noeud) {
  // Entree : un noeud, un fichier et le pointeur vers un numero
  // Sortie : La fonction ecrit dans le fichier f les noeuds et les aretes
  // de l'arbre racine.
  if(racine == NULL){
    return;
  }
  *numero_noeud = *numero_noeud + 1;
  racine->id = *numero_noeud;
  fprintf(f, "%d [label=\"%d,%d\",fontcolor=webmaroon, color=grey%d];\n", racine->id, racine->nb_visites, racine->nb_victoires, couleur_gris(racine));
  for (int i = 0; i < 6; i++) {
    parcours_en_profondeur(racine->enfants[i], f, numero_noeud);
    if(racine->enfants[i] != NULL){
      int si_joueur_2 = 0;
      if(racine->pos->joueur == 2){si_joueur_2 = 6;}
      fprintf(f, "%d -- %d [label=\"%d\"];\n", racine->id, racine->enfants[i]->id,i+si_joueur_2);
    }
  }
}

void affichage_arbre(node* arb){
  // Entree : Un arbre
  // Sortie : Creation et affichage d'un fichier pour visualiser l'arbre
  const char* nom_fichier = "arbre.dot";
  FILE* f = fopen(nom_fichier, "w");
  fprintf(f, "graph arbre {\n node [shape=circle,style=filled];\n");
  int numero = -1;
  parcours_en_profondeur(arb, f, &numero);
  fprintf(f,"}\n");
  fclose(f);
  system("dot -Tpng arbre.dot -o arbre.png");
  system("xdg-open arbre.png");
}

int strategie_mcts(position* p){
  clock_t start = clock();
  int si_joueur_2 = 0;
  if(p->joueur == 2){si_joueur_2=6;}
  node* racine = mcts(p);
  int meilleur_coup = -1;
  double max = -1.;
  for(int i = 0; i < 6; i++){
    if(racine->enfants[i] != NULL){
      double ratio = racine->enfants[i]->nb_victoires / (double)(racine->enfants[i]->nb_visites);
      if(ratio > max){
	meilleur_coup = i;
	max = ratio;
      }
    }
  }
  clock_t end = clock();
  double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Le temps d'exécution de la strategie Monte-Carlo est de %f secondes\n", cpu_time_used);
  return meilleur_coup + si_joueur_2;
}

void test(position* pos){
  node* racine = mcts(pos);
  affichage_arbre(racine);
}
