#include "mcts.h"

static noeud* creer_noeud(position* pos){
  // Entree : Une position ie un etat du jeu
  // Sortie : Un noeud de l'arbre de recherche ayant un nb de visites et
  // une valeur de Q = 0. avec des fils nuls
  noeud* res = malloc(sizeof(noeud));
  res->id = -1;
  res->pos = pos;
  res->nb_visites = 0;
  res->Q = 0.;
  res->parent = NULL;
  for(int i = 0; i < 6; i++){
    res->enfants[i] = NULL;
  }
  return res;
}

static noeud* developpable(noeud* n){
  // Entree : Un noeud n non terminale
  // Sortie : Un noeud f non joue depuis n ou NULL si tous les coups sont joues (ie l'arbre ne peut pas etre developpe)
  int si_joueur_2 = 0;
  if(n->pos->joueur == 2){si_joueur_2 = 6;}
  int nb_coups_dispo = 0;
  for(int i = 0; i < 6; i++){
    if(n->enfants[i] != NULL){printf("%d non null\n",i);}// debug
    if(n->enfants[i] == NULL && puit_jouable(n->pos, i + si_joueur_2)){
      nb_coups_dispo++;
    }
  }
  if(nb_coups_dispo == 0){ // Tous les coups ont ete explores
    return NULL;
  }
  int coup_hasard = rand()%nb_coups_dispo;
  for(int i = 0; i < 6; i++){
    if(n->enfants[i] == NULL && puit_jouable(n->pos, i + si_joueur_2)){
      if(coup_hasard == 0){
	position* pos_apres_coup = copie_pos(n->pos);
	jouer_coup(pos_apres_coup, i + si_joueur_2);
	noeud* fils = creer_noeud(pos_apres_coup);
	n->enfants[i] = fils;
	fils->parent = n;
	return fils;
      }else{
	coup_hasard--;
      }
    }
  }
  printf("mcts | developpable | Erreur dans la fonction\n");
  assert(false);
  return NULL;
}

noeud* meilleur_enfant(noeud* n, double c){
  int indice_max = -1;
  double max = -9999999.;
  double epsilon = 0.000001;
  for(int i = 0; i < 6; i++){
    if(n->enfants[i] != NULL){
      double score = (n->enfants[i]->Q / (n->enfants[i]->nb_visites + epsilon)) + c * (sqrt((2 * log(n->nb_visites + epsilon)) / (n->enfants[i]->nb_visites + epsilon))); // On ajoute epsilon pour garantir la convention /0 = +inf
      printf("score %d : %lf\n", i, score); // debug
      if(score > max){
	printf("HIHIHIH\n"); //debug
	indice_max = i;
	max = score;
      }
    }
  }
  if(indice_max == -1){
    printf("mcts.c | meilleur_enfant | Erreur tous les fils sont NULL\n");
    assert(indice_max != -1);
  }
  return n->enfants[indice_max];
}

static noeud* regle_arbre(noeud* racine){
  // Entree : la racine de l'arbre d'exploration
  // Sortie : Un noeud choisi en utilisant la methode UCT
  noeud* n = racine;
  while(!terminale(n->pos)){
    noeud* fils_a_etendre = developpable(n);
    afficher_position(fils_a_etendre->pos); //debug
    if(fils_a_etendre == NULL){ // Tous les coups ont ete joues
      printf("Je renvoie le meilleur enfant\n");
      n = meilleur_enfant(fils_a_etendre, CONSTANTE);
    }else{
      return fils_a_etendre;
    }
  }
  return n;
}

static int regle_default(noeud* noeud, int joueur){
  // Entree : Un noeud
  // Sortie : Joue une partie au hasard a partir de la position du noeud
  // et renvoie le score
  position* copie = copie_pos(noeud->pos);
  int gagnant = jouer_partie(copie, strategie_hasard, strategie_hasard, false);
  liberer_position(copie);
  if(gagnant == joueur){
    return 1;
  }else if(gagnant == 3 - joueur){
    return -1;
  }else{
    return 0;
  }
}

static void retropropagation(noeud* n, int joueur){
  int score = regle_default(n, joueur);
  while(n != NULL){
    n->nb_visites++;
    n->Q = ((n->nb_visites - 1)*n->Q + score)/(double)(n->nb_visites);
    n = n->parent;
  }
}

noeud* mcts_tree(position* pos){
  noeud* racine = creer_noeud(pos);
  for(int i = 0; i < CONTRAINTE_MCTS; i++){
    printf("Passage %d\n", i); // debug
    noeud* a_visiter = regle_arbre(racine);
    retropropagation(a_visiter, racine->pos->joueur);
  }
  return meilleur_enfant(racine, 0.);
}


int couleur_gris(noeud* n){
  // Entree : Un noeud
  // Sortie : Un nombre dans [|0;100|] qui correspond a une nuance de gris
  // choisie par rapport au winrate
  return (int)((n->Q+1)/2);
}


static void parcours_en_profondeur(noeud* racine, FILE* f, int* numero_noeud){
  // Entree : un noeud, un fichier et le pointeur vers un numero
  // Sortie : La fonction ecrit dans le fichier f les noeuds et les aretes
  // de l'arbre racine.
  if(racine == NULL){
    return;
  }
  *numero_noeud = *numero_noeud + 1;
  racine->id = *numero_noeud;
  fprintf(f, "%d [label=\"%d,%lf\",fontcolor=red, color=grey%d];\n", racine->id, racine->nb_visites, racine->Q, couleur_gris(racine));
  for (int i = 0; i < 6; i++) {
    parcours_en_profondeur(racine->enfants[i], f, numero_noeud);
    if(racine->enfants[i] != NULL){
      int si_joueur_2 = 0;
      if(racine->pos->joueur == 2){si_joueur_2 = 6;}
      fprintf(f, "%d -- %d [label=\"%d\"];\n", racine->id, racine->enfants[i]->id,i+si_joueur_2);
    }
  }
}

static void affichage_arbre(noeud* arb){
  // Entree : Un arbre
  // Sortie : Creation et affichage d'un fichier pour visualiser l'arbre
  const char* nom_fichier = "arbre_mcts.dot";
  FILE* f = fopen(nom_fichier, "w");
  fprintf(f, "graph arbre {\n node [shape=circle,style=filled,fontname=\"Helvetica\",fontsize=20];\nedge [fontname=\"Helvetica\"]\n;");
  int numero = -1;
  parcours_en_profondeur(arb, f, &numero);
  fprintf(f,"}\n");
  fclose(f);
  system("dot -Tpdf arbre_mcts.dot -o arbre_mcts.pdf");
  system("xdg-open arbre_mcts.pdf");
}



void test(){
  position* pos = creer_position_fichier("grilles/grille1.txt", 1, 0, 0);
  noeud* racine = mcts_tree(pos);
  for(int i = 0; i < 6; i++){
    if(racine->enfants[i] != NULL){printf("1 enfants\n");}
  }
}
