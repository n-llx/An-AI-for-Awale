type configuration = int array;;

(** Renvoie le coefficient binomial k parmi n**)
let binom n k =
  let num = ref 1 in
  let denum = ref 1 in
  for i = 1 to k do
    num := !num * (n - i + 1);
    denum := !denum * i;
  done;
  !num / !denum
;;

(** Renvoie le vecteur c definit au debut de la partie 2**)
let vecteur_c (n_config : configuration) =
  let c = Array.make 11 0 in
  c.(0) <- n_config.(0);
  for i = 1 to 10 do
    c.(i) <- n_config.(i) + 1 + c.(i-1);
  done;
  c
;;

(** Prend en entree un vecteur c et renvoie enc(c) **)
let enc c : int =
  let res = ref 0 in
  for i = 1 to 11 do
    res := !res + (binom c.(i-1) i);
  done;
  !res
;;

(** Renvoie le plus grand entier n tel que k parmis n soit inferieur a m **)
let plus_grand_coeff k m =
  if m < 0 then 0 else (
    let n = ref k in
    while binom !n k <= m do
      incr n;
    done;
    !n - 1
  )
;;

(** Renvoie le vecteur c decode **)
let dec g =
  let c = Array.make 11 0 in
  let g = ref g in
  for i = 10 downto 0 do
    let c_i = plus_grand_coeff (i+1) !g in
(*    Printf.printf "c_%d = %d et g%d =%d\n" i c_i (i+1) !g; *)
    g := !g - (binom c_i (i+1));
    c.(i) <- c_i;
  done;
  c
;;

let extractNombreGodel epsilon =
  let nb_pierre = epsilon mod 49 in
  let enc_c = epsilon / 49 in
  (nb_pierre, enc_c )
;;

(** Cette fonction renvoie la configuration associee a un certain nombre de godel g**)
let godelVersConfig g =
  let nb_pierre, enc_c = extractNombreGodel g in
  let c = dec enc_c in
  let nb_pierre_placee = ref c.(0) in
  let a = Array.make 12 0 in
  a.(0) <- c.(0);
  for i = 1 to 10 do
    a.(i) <- c.(i) - 1 - c.(i-1);
    nb_pierre_placee := !nb_pierre_placee + a.(i);
  done;
  a.(11) <- nb_pierre - !nb_pierre_placee;
  a
;;

(** Affiche une configuration**)
let printConfig (a : configuration) =
  for i = 11 downto 6 do
    Printf.printf "%d " a.(i);
  done;
  Printf.printf "\n";
  for i = 0 to 5 do
    Printf.printf "%d " a.(i);
  done;
 Printf.printf "\n\n";
;;

(** Affiche la configuration de nombre de Godel x**)
let grandD x = printConfig (godelVersConfig x);;


(**Modifie la configuration et renvoie l'indice du dernier puit**)
let semaille (c : configuration) (k : int) : int =
  let pieresAJouer = ref c.(k) in
  (*Le premier puit qui recoit la pierre est le suivant*)
  let puitsReceveur = ref ((k + 1) mod 12) in
  (*On vide le puit joue*)
  c.(k) <- 0;
  while !pieresAJouer > 0 do
    (*Si on passe par le puit inital, on le saute*)
    if !puitsReceveur <> k then (
      c.(!puitsReceveur) <- c.(!puitsReceveur) + 1;
      puitsReceveur := (!puitsReceveur + 1) mod 12;
      decr pieresAJouer;
    )
    else
      puitsReceveur := (!puitsReceveur + 1) mod 12;
  done;
  (*On renvoie la position du puit ou la derniere graine a ete placee*)
  (!puitsReceveur - 1) mod 12
;;

(**Modifie la configuration et renvoie le nombre de pierres recoltées**)
let recolte (c : configuration) (k : int) : int =
  assert(k <= 12 && k >= 0);
  let grainesRecoltees = ref 0 in
  let indicePuit = ref k in
  (* Tant que je suis dans le camp adverse, dans un puit contenant 2 ou 3 graines*)
  while !indicePuit >= 6 && (c.(!indicePuit) = 2 || c.(!indicePuit) = 3) do
    grainesRecoltees := !grainesRecoltees + c.(!indicePuit);
    c.(!indicePuit) <- 0;
    decr indicePuit;
  done;
  !grainesRecoltees
;;

exception PuitVide;;

(** Si le coup est invalide, leve l'exception PuitVide silon on joue le coup et on renvoie la configuration resultante et le nombre de pierres capturees **)
let coup (c : configuration) (k : int) : (configuration * int) =
  assert(k <= 12 && k >= 0);
  let copy = Array.copy c in
  if copy.(k) = 0 then raise PuitVide
  else
    let puitArrivee = semaille copy k in
    (copy, recolte c puitArrivee)
;;

(** Renvoie vrai si tous les puits adverses sont vides**)
let famine (c : configuration) : bool =
  let res = ref true in
  for i = 6 to 11 do
    res := !res && (c.(i) = 0);
  done;
  !res
;;

(** Renvoie la liste des puits jouables pour le joueur Sud **)
let puits_jouables (c : configuration) : int list =
  let jouables = ref [] in
  for i = 0 to 5 do
    if c.(i) <> 0 then
      begin
        let (suivant, _) = coup c i in
        let fam = famine suivant in
        if not fam then
          jouables := i :: !jouables
        else
          ()
      end;
  done;
  !jouables
;;

(** Renvoie vraie si le joueur Sud peut jouer **)
let jouable_joueur (c : configuration) : bool =
  ((puits_jouables c) <> [])
;;

(** Renvoie le nombre de Godel d'une configuration**)
let godel (c : configuration) : int =
  let vecteur = vecteur_c c in
  let enc_c = enc vecteur in
  let n = Array.fold_left (+) 0 c in
  n + 25 * enc_c
;;

(** Retourne le plateau : le joueur Sud devient le joueur Nord et inversement**)
let retourne (c : configuration) : configuration =
  let cpy = Array.copy c in
  for i = 0 to 11 do
    cpy.(i) <- c.((i + 6) mod 12)
  done;
  cpy
;;

(** Renvoie vrai si on recolte en jouant le puit p**)
let coup_mene_recolte (c : configuration) (p : int) : bool =
  let (_, a) = coup c p in
  a <> 0
;;

(** Renvoie vrai si le puit p est jouable**)
let coup_jouable (c : configuration) (p : int) : bool =
  let coups_possibles = puits_jouables c in
  List.mem p coups_possibles
;;


let liste_anti_coup_puit (c : configuration) (p : int) : configuration list=
  assert(c.(p) = 0);
  let ajouter = ref true in
  let res = ref [] in
  let ancienne_config = ref c in
  let compteur = ref 1 in
  let n = 12 in
  while !ajouter do
    let nouvelle_config = Array.copy !ancienne_config in
    nouvelle_config.(p) <- nouvelle_config.(p) + 1;
    let supp_indice = (p + !compteur) mod n in
    if supp_indice <> p then (
        nouvelle_config.(supp_indice) <- nouvelle_config.(supp_indice) - 1;
        if not (coup_mene_recolte nouvelle_config p) && coup_jouable nouvelle_config p && nouvelle_config.(supp_indice) >= 0 && !compteur < 50 then (
            incr compteur;
            res := nouvelle_config :: !res;
            ancienne_config := nouvelle_config
          )
        else 
          if nouvelle_config.(supp_indice) >= 0 && !compteur < 50 then
            (
              incr compteur;
              ancienne_config := nouvelle_config;
            )
          else (
              ajouter := false
            )
      )
    else (
        incr compteur;
        nouvelle_config.(p) <- nouvelle_config.(p) - 1;
        ancienne_config := nouvelle_config;
      )
  done;
  !res
;;

(**Renvoie une liste des coups menant a la configuration c**)
(**On suppose que le joueur qui va jouer est au Sud**)
let liste_anti_coups (conf : configuration) =
  let c = retourne conf in 
  let n = 6 in 
  let res = ref [] in
  let ajoute_elts_liste tableau =
    List.iter (fun a -> res := a :: !res;) tableau;
  in
  for i = 0 to n-1 do
    if c.(i) = 0 then
      (
        ajoute_elts_liste (liste_anti_coup_puit c i);
    )
  done;
  !res
;;

(** Renvoie vrai si la position est valide ie ne contient pas de nb de pierres < 0**)
let valide godel =
  (*Renvoie vrai si la position est valide*)
  not (Array.exists (fun x -> x < 0) (godelVersConfig godel))
;;

(** Compte le nombre de pierres **)
let nb_pierre_godel (godel : int) =
  let config = godelVersConfig godel in
  Array.fold_left (+) 0 config
;;      

(** On suppose que l'on connait les scores des configurations dont le nombre de pierres est inf a n et on construit celle pour n**)
let initialise_bdd (n : int) bdd : unit =
  
  
