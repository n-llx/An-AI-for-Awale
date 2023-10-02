let u0 = 42;;
let a = 1103515245;;
let c = 12345;;
let m = 1 lsl 31;;

type configuration = int array;;

let partie_entiere x =
  int_of_float (floor x)
;;  

let rec u n =
  if n = 0 then u0
  else let u_prec = u (n - 1) in
    (a * u_prec + c) mod m
;; 

let v7 n =
  let u = float_of_int (u n) in
  let m_f = float_of_int m in
  partie_entiere (3432. *. (u /. m_f))
;;  

let v24 n  =
  let u = float_of_int (u n) in
  let m = float_of_int m in
  partie_entiere (2629575. *. u /. m)
;;  

let w7 n =
  7 + 25 * (v7 n)
;; 

let w24 n =
  24 + 25 * (v24 n)
;; 

let q1_a = (u 1 mod 37, v7 1 mod 37, w7 1 mod 37) ;;
let q1_b = (u 1000 mod 37, v24 1000 mod 37, w24 1000 mod 37);;
let q1_c = (u 1000000 mod 37, v24 1000000 mod 37, w24 1000000 mod 37);;


let binom n k =
  let num = ref 1 in
  let denum = ref 1 in
  for i = 1 to k do
    num := !num * (n - i + 1);
    denum := !denum * i;
  done;
  !num / !denum
;;

let q2_a =
  let i = 4 + ((u 10) mod 4) in
  let j = i + 5 + ((u 11) mod 20) in
  binom j i
;;

let q2_b = 
  let i = 4 + ((u 12) mod 4) in
  let j = i + 5 + ((u 13) mod 20) in
  binom j i
;;

let q2_c = 
  let i = 4 + ((u 14) mod 4) in
  let j = i + 5 + ((u 15) mod 20) in
  binom j i
;;

let vecteur_c (n_config : configuration) =
  let c = Array.make 7 0 in
  c.(0) <- n_config.(0);
  for i = 1 to 6 do
    c.(i) <- n_config.(i) + 1 + c.(i-1);
  done;
  c
;;

let ex1 = [|2;1;4;0;1;5;2;4|];;

let enc c : int =
  let res = ref 0 in
  for i = 1 to 7 do
    res := !res + (binom c.(i-1) i);
  done;
  !res
;;

let plus_grand_coeff k m =
  if m < 0 then 0 else (
    let n = ref k in
    while binom !n k <= m do
      incr n;
    done;
    !n - 1
  )
;;

let dec g =
  let c = Array.make 7 0 in
  let g = ref g in
  for i = 6 downto 0 do
    let c_i = plus_grand_coeff (i+1) !g in
(*    Printf.printf "c_%d = %d et g%d =%d\n" i c_i (i+1) !g; *)
    g := !g - (binom c_i (i+1));
    c.(i) <- c_i;
  done;
  c
;;
(*
let q3_a = dec (v7 1);;
let q3_b = dec (v7 2);;
let q3_c = dec (v7 3);;
let q3_d = dec (v24 4);;
let q3_e = dec (v24 5);;
let q3_f = dec (v24 6);;
*)


let extractNombreGodel epsilon =
  let nb_pierre = epsilon mod 25 in
  let enc_c = epsilon / 25 in
  (nb_pierre, enc_c )
;;  

let godelVersConfig g =
  let nb_pierre, enc_c = extractNombreGodel g in
  let c = dec enc_c in
  let nb_pierre_placee = ref c.(0) in
  let a = Array.make 8 0 in
  a.(0) <- c.(0);
  for i = 1 to 6 do
    a.(i) <- c.(i) - 1 - c.(i-1);
    nb_pierre_placee := !nb_pierre_placee + a.(i);
  done;                         
  a.(7) <- nb_pierre - !nb_pierre_placee;
  a
;;  

let printConfig (a : configuration) =
  for i = 7 downto 4 do
    Printf.printf "%d " a.(i);
  done; 
  Printf.printf "\n";
  for i = 0 to 3 do
    Printf.printf "%d " a.(i);
  done; 
 Printf.printf "\n";
;;
 
let question4_valeur = [w7 1; w7 2; w7 3; w24 4; w24 5; w24 6];;
let question4 liste =
  let sousQ = ref 'a' in
  let add_char x = x := char_of_int (int_of_char !x + 1) in
  let f x =
    Printf.printf "%c)\n" !sousQ;
    add_char sousQ;
    printConfig (godelVersConfig x);
  in List.iter f liste
;;
(*question4 question4_valeur;;*)
    
let grandD x = printConfig (godelVersConfig x);;

let semaille (c : configuration) (k : int) : int =
  (*Renvoie la config apres semaille et derniere case ou graine pose*)
  let pieresAJouer = ref c.(k) in
  let puitsReceveur = ref ((k + 1) mod 8) in
  c.(k) <- 0;
  while !pieresAJouer > 0 do
    if !puitsReceveur <> k then (
      c.(!puitsReceveur) <- c.(!puitsReceveur) + 1;
      puitsReceveur := (!puitsReceveur + 1) mod 8;
      decr pieresAJouer;
    )
    else
      puitsReceveur := (!puitsReceveur + 1) mod 8;
  done;
  (!puitsReceveur - 1) mod 8
;;

let recolte (c : configuration) (k : int) : int =
  let grainesRecoltees = ref 0 in
  let indicePuit = ref k in
  while !indicePuit >= 4 && (c.(!indicePuit) = 2 || c.(!indicePuit) = 3) do
    grainesRecoltees := !grainesRecoltees + c.(!indicePuit);
    c.(!indicePuit) <- 0;
    decr indicePuit;
  done;
  !grainesRecoltees
;;

exception PuitVide;;

let coup (c : configuration) (k : int) : int =
  if c.(k) = 0 then raise PuitVide
  else
    let puitArrivee = semaille c k in
    recolte c puitArrivee
;;

let estPossible (c : configuration) : bool =
  let res = ref true in
  for i = 0 to 3 do
    res := !res && (c.(i) = 0);
  done;
  not !res
;;

let famine (c : configuration) : bool = 
  let res = ref true in
  for i = 4 to 7 do
    res := !res && (c.(i) = 0);
  done;
  !res
;;


exception Impossible;;
let puitsJouables (c : configuration) : int list =
  let jouables = ref [] in
  for i = 0 to 3 do
    try
      let plateau = Array.copy c in
      let _ = coup plateau i in
      let fam = famine plateau in
      if not fam then
        jouables := i :: !jouables
      else 
        ()
    with
    | PuitVide -> ()
  done;
  !jouables
;;

let jouable (c : configuration) : bool =
  ((puitsJouables c) <> []) 
;;

let troisPremierNonJouable () : int list =
  let compteur = ref 0 in
  let res = ref [] in
  let configuration = ref (godelVersConfig (w7 !compteur)) in
  while !compteur < 10000 && List.length !res < 3 do
    incr compteur;
    configuration := godelVersConfig (w7 !compteur);
    if not (jouable !configuration) && (estPossible !configuration) then 
      res := !compteur :: !res
    else
      ()
  done;
  !res
;;
(*troisPremierNonJouable () *)

let question6 (i : int) : int * int =
  let compteur = ref 0 in
  let condition = ref true in
  let config = ref [||] in
  while !compteur < 10000 && !condition do
    incr compteur;
    config := godelVersConfig (w24 !compteur);
    condition := List.length (puitsJouables !config) <> i;
  done;
  let grainesRamassees =
    List.map (fun x -> coup (Array.copy !config) x) (puitsJouables !config) in
  let maxGraine = List.fold_left max (-1) grainesRamassees in
  (!compteur, maxGraine)
;;

let question7 () =
  let compteur = ref 0 in
  let configuration = ref (godelVersConfig (w24 !compteur)) in
  let res = ref [] in
  while !compteur < 10000 && List.length !res <> 3 do
    incr compteur;
    configuration := godelVersConfig (w24 !compteur);
    if not (jouable (godelVersConfig (w24 !compteur))) then
      res := !compteur :: !res
    else 
      ()
  done;
  !res
;;

let godel (c : configuration) : int =
  let vecteur = vecteur_c c in
  let enc_c = enc vecteur in
  let n = Array.fold_left (+) 0 c in
  n + 25 * enc_c
;;

let retourne (c : configuration) : unit =
  let cpy = Array.copy c in
  for i = 0 to 7 do
    c.(i) <- cpy.((i + 4) mod 8)
  done;
;;

let q8a = 
